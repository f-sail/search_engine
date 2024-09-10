#include "../include/PageLibPreprocessor.h"
#include "../include/Configuration.h"
#include "../include/simhash/Simhasher.hpp"
#include "../include/tinyxml2.h"
#include "../include/SplitTool.h"

#include <bits/getopt_core.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

#define TOPN 5
#define BUFF_SIZE 4096

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::istringstream;


PageLibPreprocessor::PageLibPreprocessor()
: _wordCutter(new SplitToolCppJieba())
{
    loadOffset();
    return;
}

PageLibPreprocessor::~PageLibPreprocessor(){
    storeOnDisk();
    if(_wordCutter){
        delete _wordCutter;
    }
    return;
}

void PageLibPreprocessor::cutRedundantPage(){
    simhash::Simhasher simhasher("../include/simhash/dict/jieba.dict.utf8",
                                 "../include/simhash/dict/hmm_model.utf8",
                                 "../include/simhash/dict/idf.utf8",
                                 "../include/simhash/dict/stop_words.utf8");
    ifstream ifs(Configuration::getInstance()->getConfigMap()["path_ripepage.dat"]);
    if(!ifs){
        std::cerr << "ofstream failed\n";
        return;
    }
    ofstream ofs(Configuration::getInstance()->getConfigMap()["path_newripepage.dat"]);
    if(!ofs){
        std::cerr << "ofstream failed\n";
        return;
    }
    for(auto& pair: _offsetLib){
        ifs.seekg(pair.second.first);
        int doc_size = pair.second.second - pair.second.first + 1;
        int ret = 0;
        string doc;
        while(ret < doc_size){
            char buff[BUFF_SIZE] = {0};
            ifs.read(buff, doc_size - ret < BUFF_SIZE - 1? doc_size - ret: BUFF_SIZE - 1);
            ret += ifs.gcount();
            doc += buff;
        }
        analyze(doc, ofs, simhasher);
    }
    ifs.close();
    ofs.close();
    //for(auto i: _new_offsetLib){
    //    cout << i.first << " " << i.second.first << " " << i.second.second << "\n";
    //}
    return;
}

void PageLibPreprocessor::buildInvertIndexMap(){
    int N = _new_offsetLib.size();
    for(const auto& word_info: _wordsInfo){
        string word = word_info.first;
        int DF = word_info.second.DF;

        for(const auto& pair: word_info.second.docTF){
            int docid = pair.first;
            int TF = pair.second;

            double IDF = log2((double)N / (DF + 1) + 1);
            double w = TF * IDF;

            _invertIndexLib[word].emplace(docid, w);
        }
    }
    return;
}

void PageLibPreprocessor::storeOnDisk(){
    cout << ">> Storing...\n";
    /* newoffset.dat */{
        string path = Configuration::getInstance()->getConfigMap()["path_newoffset.dat"];
        cout << " |" << path << "\n";
        ofstream ofs(path);
        for(const auto& pair: _new_offsetLib){
            ofs << pair.first << " "
                << pair.second.first << " "
                << pair.second.second << "\n";
        }
        ofs.close();
    }
    /* invertindex.dat */{
        string path = Configuration::getInstance()->getConfigMap()["path_invertindex.dat"];
        cout << " |" << path << "\n";
        ofstream ofs(path);
        for(const auto & pair: _invertIndexLib){
            ofs << pair.first << "    ";
                for(const auto& p: pair.second){
                    ofs << p.first << " "
                        << p.second << "    ";
                }
            ofs << "\n";
        }
        ofs.close();
    }
    cout << ">> Storing completed!\n";
    return;
}

bool PageLibPreprocessor::isSimilar(uint64_t value){
    for(uint64_t u64: _pageList){
        if(simhash::Simhasher::isEqual(u64, value, 5)){
            return true;
        }
    }
    return false;
}

void PageLibPreprocessor::loadOffset(){
    ifstream ifs(Configuration::getInstance()->getConfigMap()["path_offset.dat"]);
    if(!ifs){
        std::cerr << "ifstream failed\n";
        return;
    }
    string line;
    while(getline(ifs,line)){
        istringstream iss(line);
        string docid, begin, end;
        iss >> docid;
        iss >> begin;
        iss >> end;
        std::pair<int,int> pair(atoi(begin.c_str()), atoi(end.c_str()));
        _offsetLib.emplace(atoi(docid.c_str()), pair);
    }
    ifs.close();
    return;
}

void PageLibPreprocessor::analyze(const string& doc, ofstream& ofs, simhash::Simhasher& simhasher){
    size_t pos_docid_begin = doc.find("<docid>") + strlen("<docid>");
    size_t pos_docid_end = doc.find("</docid>");
    string docid(doc.substr(pos_docid_begin, pos_docid_end - pos_docid_begin));
    
    size_t pos_url_begin = doc.find("<url>", pos_docid_end) + strlen("<url>");
    size_t pos_url_end = doc.find("</url>");
    string url(doc.substr(pos_url_begin, pos_url_end - pos_url_begin));
    
    size_t pos_title_begin = doc.find("<title>", pos_url_end) + strlen("<title>");
    size_t pos_title_end = doc.find("</title>");
    string title(doc.substr(pos_title_begin, pos_title_end - pos_title_begin));
    
    size_t pos_content_begin = doc.find("<content>", pos_title_end) + strlen("<content>");
    size_t pos_content_end = doc.find("</content>");
    string content(doc.substr(pos_content_begin, pos_content_end - pos_content_begin));


    uint64_t value = 0;
    simhasher.make(content, TOPN, value);
    if(!isSimilar(value)){
        std::pair<int, int> pair;
        pair.first = ofs.tellp();
        ofs << "<doc>\n\t<docid>"       << docid
            << "</docid>\n\t<url>"      << url
            << "</url>\n\t<title>"      << title
            << "</title>\n\t<content>"  << content
            << "</content>\n</doc>";
        pair.second = ofs.tellp();
        ofs << "\n";
        
        _pageList.push_back(value);
        _new_offsetLib.emplace(atoi(docid.c_str()), pair);

        wordStatistics(atoi(docid.c_str()),content);
    }

    return;
}

void PageLibPreprocessor::wordStatistics(int docid, string content){
    vector<string> words(_wordCutter->cut(content));
    for(const string& word: words){
        /* 是否是在所有文章中第一次出现 */
        if(0 == _wordsInfo.count(word)){
            _wordsInfo[word].DF = 1;
            _wordsInfo[word].docTF[docid] = 1;
        
        }
        /* 是否是在本文章中第一次出现 */
        if(0 == _wordsInfo[word].docTF.count(docid)){
            ++_wordsInfo[word].DF;
            _wordsInfo[word].docTF[docid] = 1;
        }else{
            ++_wordsInfo[word].docTF[docid];
        }
    }
    return;
}