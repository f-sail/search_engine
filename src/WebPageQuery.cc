#include "../include/WebPageQuery.h"
#include "../include/SplitTool.h"
#include "../include/Configuration.h"

#include <algorithm>
#include <exception>
#include <fstream>
#include <iterator>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <utility>

#define BUFF_SIZE 65535
/* #define BUFF_SIZE 256 */

using std::set;
using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;


WebPageQuery* WebPageQuery::_pInstance = nullptr;

WebPageQuery::WebPageQuery()
: _cutter(new SplitToolCppJieba())
, _ifs(Configuration::getInstance()->getConfigMap()["path_newripepage.dat"])
{
    Loading();
    return;
}
WebPageQuery::~WebPageQuery(){
    if(_pInstance){
        delete _pInstance;
        _pInstance = nullptr;
    }
    if(_cutter){
        delete _cutter;
        _cutter = nullptr;
    }
    if(_ifs.is_open()){
        _ifs.close();
    }
    return;
}

WebPageQuery* WebPageQuery::getInstance(){
    if(!_pInstance){
        _pInstance = new WebPageQuery();
    }
    return _pInstance;
}

WebPageQuery::IndexTable WebPageQuery::doQuery(std::string key){
    vector<string> ret = _cutter->cut(key);
    std::set<string> words(ret.begin(), ret.end());
    set<int> docids(getDocids(words));
    
    std::map<string, int> wordFerq;
    for(const string& word: ret){
        if(wordFerq.count(word)){
            ++wordFerq[word];
        }else{
            wordFerq[word] = 1;
        }
    }
    IndexTable table;
    for(const std::pair<string, int>& pair: wordFerq){
        table[pair.first][0] = static_cast<double>(pair.second) / ret.size();
        for(int i: docids){
            table[pair.first][i] = _indexLib[pair.first][i];
        }
    }
    return table;
}

std::string WebPageQuery::getPage(int docid){
    string doc;
    std::pair<int, int> pos = _offsetLib[docid];
    int len = pos.second - pos.first + 1;
    _ifs.seekg(pos.first);
    char buff[BUFF_SIZE] = {0};
    while(len > 0){
        _ifs.read(buff, BUFF_SIZE < len ? BUFF_SIZE : len);
        size_t ret = _ifs.gcount();
        len -= ret;
        doc += buff;
    }
    return doc;
}

std::set<int> WebPageQuery::getDocids(const std::set<std::string>& words){
    set<int> docids;
    for(const auto& pair: _indexLib[*words.begin()]){
        docids.emplace(pair.first);
    }
    for(const string& word: words){
        set<int> tmp;
        for(const auto& pair: _indexLib[word]){
            if(docids.count(pair.first)){
                tmp.emplace(pair.first);
            }
        }
        docids = std::move(tmp);
    }
    return docids;
}

void WebPageQuery::Loading(){
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
                _indexLib[word][stoi(docid)] = std::stod(weight);
            }
        }
        ifs.close();
    }
    return;
}
