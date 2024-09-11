#include "../include/Configuration.h"
#include "../include/log4cpp.h"
#include "../include/Dictionary.h"
#include "../include/SplitTool.h"

#include <fstream>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

using std::set;
using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;

Dictionary* Dictionary::_pInstance = nullptr;


Dictionary * Dictionary::getInstance(){
    if(!_pInstance){
        _pInstance = new Dictionary();
    }
    return _pInstance;
}

vector<std::pair<string, int>> Dictionary::doQuery(const string& key){
    vector<std::pair<string, int>> vec;
    for(int i = 0; i < key.size();){
        size_t n = SplitTool::nBytesCode(key[i]);
        for(int idx: _index[key.substr(i,n)]){
            vec.emplace_back(_dict[idx - 1].first, _dict[idx - 1].second);
        }
        i += n;
    }
    return vec;
}

Dictionary::Dictionary(){
    Loading();
    return;
}

Dictionary::~Dictionary(){
    if(_pInstance){
        delete _pInstance;
        _pInstance = nullptr;
    }
    return;
}

void Dictionary::Loading(){
    std::cout << ">> Loading ...\n";
    /* dict.dat */{
        string path = Configuration::getInstance()->getConfigMap()["path_dict.dat"];
        std::cout << " |" << path << "\n";
        ifstream ifs(path);  
        string line;
        while(getline(ifs, line)){
            istringstream iss(line);
            string word,freq;
            iss >> word >> freq;
            _dict.emplace_back(word, atoi(freq.c_str()));
        }
        ifs.close();
    }
    /* dictindex.dat */{
        string path = Configuration::getInstance()->getConfigMap()["path_dictindex.dat"];
        std::cout << " |" << path << "\n";
        ifstream ifs(path);  
        string line;
        while(getline(ifs, line)){
            istringstream iss(line);
            string word, idx;
            iss >> word;
            while(iss >> idx){
                _index[word].emplace(atoi(idx.c_str()));
            }
        }
        ifs.close();
    }
    std::cout << ">> Loading completed!\n";
}

/* int Dictionary::distance(std::string candidate){} */
