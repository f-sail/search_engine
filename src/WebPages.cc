#include "../include/WebPages.h"
#include "../include/SplitTool.h"
#include "../include/Configuration.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>

using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;


WebPages* WebPages::_pInstance = nullptr;

WebPages::WebPages()
: _cutter(new SplitToolCppJieba())
, _ifsPage(Configuration::getInstance()->getConfigMap()["newripepage.dat"])
{
    Loading();
    return;
}
WebPages::~WebPages(){
    if(_pInstance){
        delete _pInstance;
        _pInstance = nullptr;
    }
    if(_cutter){
        delete _cutter;
        _cutter = nullptr;
    }
    return;
}
WebPages* WebPages::getInstance(){
    if(!_pInstance){
        _pInstance = new WebPages();
    }
    return _pInstance;
}
std::vector<WebPages> WebPages::doQuery(std::string key){
    return vector<WebPages>();
}
void WebPages::Loading(){
    std::cout << ">> Loading...\n";
    /* newoffset.dat */{
        string path = Configuration::getInstance()->getConfigMap()["path_newoffset.dat"];
        std::cout << " |" << path << "\n";
        ifstream ifs(path);
        string line;
        while(getline(ifs, line)){
            istringstream iss(line);
            string docid, begin, end;
            iss >> docid >> begin >> end;
            _offsetLib.emplace(atoi(docid.c_str()), std::make_pair(atoi(begin.c_str()), atoi(end.c_str())));
        }
        ifs.close();
    }
    /* invertindex.dat */{
        string path = Configuration::getInstance()->getConfigMap()["path_invertindex.dat"];
        std::cout << " |" << path << "\n";
        ifstream ifs(path);
        string line;
        while(getline(ifs, line)){
            istringstream iss(line);
            string word, docid, weight;
            iss >> word;
            while(iss >> docid >> weight){
                _indexLib[word] = std::make_pair(std::stoi(docid), std::stod(weight));
            }
        }
        ifs.close();
    }
}
