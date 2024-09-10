#include "../include/Configuration.h"
#include "../include/DicProducer.h"
#include "../include/SplitTool.h"
#include "../include/DirScanner.h"

#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <linux/limits.h>

#include <iostream>
/* using std::cout; */
/* using std::endl; */
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <fstream>
#include <sstream>


#define BUFF_SIZE 40960

using std::map;
using std::set;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::ostringstream;


DicProducer::DicProducer(std::string, SplitTool* tool) 
: _cuttor(tool)
{
    {
        DirScanner ds;
        ds.traverse(Configuration::getInstance()->getConfigMap()["path_yuliao/"]);
        _files = ds.getFiles();
    }
    //getFiles(Configuration::getInstance()->getConfigMap()["path_yuliao/"]);
    buildFreq();
    buildDict();
    createIndex();
    store();
    return;
}

DicProducer::~DicProducer(){
    if(_cuttor){
        delete _cuttor;
    }
    return;
}

void DicProducer::buildFreq(){
    std::cout << ">> Loading...\n";
    for(const string & path: _files){
        ifstream ifs(path);
        std::cout << " |" << path << "\n";
        if(!ifs){
            std::cerr << "ifstream open file failed!" << std::endl;
            return;
        }
        char buff[BUFF_SIZE] = {0};
        while(ifs.read(buff, BUFF_SIZE - 1)){
            string str(buff);
            vector<string> words(_cuttor->cut(str));
                for(string str: words){
                    for(char& c: str){
                        if((0 == (c & 0x80)) && isalpha(c)){
                            c = tolower(c);
                        }
                    }
                    if(_mFreq.count(str)){
                        ++_mFreq[str];
                    }else{
                        _mFreq[str] = 1;
                    }
                }
        }
        ifs.close();
    }
    std::cout << ">> Loading completed!\n";
    return;
}

void DicProducer::buildDict(){
    string word;
    set<string> stop_words;

    ifstream ifs_en(Configuration::getInstance()->getConfigMap()["path_stop_words_en"]);
    while(ifs_en >> word){
        stop_words.insert(word);
    }
    ifs_en.close();
    ifstream ifs_zh(Configuration::getInstance()->getConfigMap()["path_stop_words_zh"]);
    while(ifs_zh >> word){
        stop_words.insert(word);
    }
    ifs_zh.close();

    for(std::pair<string, int> pair: _mFreq){
        if(stop_words.count(pair.first)){
            continue;
        }else{
            _dict.push_back(pair);
        }
    }
    return;
}

void DicProducer::createIndex(){
    for(size_t idx_vec = 0; idx_vec < _dict.size(); ++idx_vec){
        string str = _dict[idx_vec].first;
        for(size_t idx_str = 0; idx_str < str.size();){
            size_t n =  nBytesCode(str[idx_str]);
            _index[str.substr(idx_str, n)].insert(idx_vec);
            idx_str += n;
        }
    }
    return;
}

void DicProducer::store(){
    std::cout << ">> Storing...\n";
    /* store "dict.dat" */{
        string path = Configuration::getInstance()->getConfigMap()["path_dict.dat"];
        std::cout << " |" << path << "\n";
        ofstream ofs(path);    
        if(!ofs){
            std::cerr << "ifstream open file failed!" << std::endl;
            return;
        }
        for(const std::pair<std::string, int>& pair: _dict){
            //if(pair.first == " "){
            //    continue;
            //}
            ofs << pair.first << " " << pair.second << "\n";
        }
        ofs.close();
    }

    /* store "dictindex.dat" */{
        string path = Configuration::getInstance()->getConfigMap()["path_dictindex.dat"];
        std::cout << " |" << path << "\n";
        ofstream ofs(path);
        if(!ofs){
            std::cerr << "ifstream open file failed!" << std::endl;
            return;
        }
        for(const std::pair<std::string, set<int>>& pair: _index){
            ofs << pair.first << " ";
            for(int i: pair.second){
                ofs << i << " ";
            }
            ofs << "\n";
        }
        ofs.close();
    }
    std::cout << ">> Storing completed!\n";
    return;
}

//void DicProducer::getFiles(const std::string& dir){
//    DIR * dirp = opendir(dir.data());
//    if(nullptr == dirp){
//        // todo
//        perror("opendir failed");
//        return;
//    }
//    struct dirent * entry;
//    while((entry = readdir(dirp)) != nullptr){
//        if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
//            if(entry->d_type == DT_DIR){
//                getFiles(dir + entry->d_name + '/');
//            }else if(entry->d_type == DT_REG){
//                _files.push_back(dir + entry->d_name);
//            }
//        }
//    }
//    return;
//}

size_t DicProducer::nBytesCode(const char ch){
    if(ch & (1 << 7)){
        int nBytes = 1;
        for(int idx = 0; idx != 6; ++idx){
            if(ch & (1 << (6 - idx))){
                ++nBytes;
            }else{
                break;
            }
        }
        return nBytes;
    }
    return 1;
}
