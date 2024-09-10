#include "../include/PageLib.h"
#include "../include/Configuration.h"
#include "../include/DirScanner.h"
#include "../include/tinyxml2.h"

#include <cctype>
#include <regex>
#include <iostream>
#include <fstream>
#include <utility>

using std::cout;
using std::endl;
using std::string;
using std::ofstream;

PageLib::PageLib()
: _pDirScanner(new DirScanner())
{
    _pDirScanner->traverse(Configuration::getInstance()->getConfigMap()["path_webpage/"]);
    return;
}
PageLib::~PageLib(){
    if(_pDirScanner){
        delete _pDirScanner;
        _pDirScanner = nullptr;
    }
    return;
}
void PageLib::create(){
    cout << ">> Loading...\n";
    for(const string& filename:_pDirScanner->getFiles()){
        cout << " |" << filename << "\n";
        string filepath(Configuration::getInstance()->getConfigMap()["path_webpage/"] + filename);
        read(filename);
    }
    cout << ">> Loading completed!\n";
    return;
}
void PageLib::store(){
    cout << ">> Storing...\n";
    /* offset.dat */{
        string path = Configuration::getInstance()->getConfigMap()["path_ripepage.dat"];
        cout << " |" << path << "\n";
        ofstream ofs(path);
        int num = 0;
        for(const RSSIteam& rss: _pages){
            std::pair<int, int> pair;
            pair.first = ofs.tellp();
            ofs << "<doc>\n\t<docid>"       << ++num
                << "</docid>\n\t<url>"      << rss.url
                << "</url>\n\t<title>"      << rss.title
                << "</title>\n\t<content>"  << rss.content
                << "</content>\n</doc>";
            pair.second = ofs.tellp();
            ofs << "\n";
            _offsetLib.emplace(num, pair);
        }
        ofs.close();
    }
   
    /* offsetlib */{
        string path = Configuration::getInstance()->getConfigMap()["path_offset.dat"];
        cout << " |" << path << "\n";
        ofstream ofs(path);
        for(const std::pair<int, std::pair<int,int>>& pair: _offsetLib){
            ofs << pair.first << " "
                << pair.second.first << " "
                << pair.second.second << "\n";
        }
        ofs.close();
    }
    cout << ">> Storing completed!\n";
    return;
}
void PageLib::read(const std::string& filepath){
    using namespace tinyxml2;
    XMLDocument doc;
    doc.LoadFile(filepath.c_str());
    if(doc.ErrorID()){
        std::cerr << "Error: " << filepath << " loaded fail" << endl;
        return;
    }
    XMLElement *itemNode = doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
    if(nullptr == itemNode){
        return;
    }
    while(itemNode){
        RSSIteam rss;
        XMLElement *tmp_itemNode = nullptr;
        /* rss.title = itemNode->FirstChildElement("title")->GetText(); */
        tmp_itemNode = itemNode->FirstChildElement("title");
        if(nullptr == tmp_itemNode){
            return;
        }
        rss.title = tmp_itemNode->GetText();
        
        /* rss.link = itemNode->FirstChildElement("link")->GetText(); */
        tmp_itemNode = itemNode->FirstChildElement("link");
        if(nullptr == tmp_itemNode){
            return;
        }
        rss.url = tmp_itemNode->GetText();
       
        /* rss.description = itemNode->FirstChildElement("description")->GetText(); */
        tmp_itemNode = itemNode->FirstChildElement("description");
        if(nullptr == tmp_itemNode){
            return;
        }
        rss.content = tmp_itemNode->GetText();
       
        std::regex reg("<[^>]+>");
        rss.content = regex_replace(rss.content, reg, "");

        _pages.emplace_back(std::move(rss));
        itemNode = itemNode->NextSiblingElement("item");
    }
}
