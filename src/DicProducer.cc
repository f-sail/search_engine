#include "../include/Configuration.h"
#include "../include/DicProducer.h"
#include "../include/SplitTool.h"

#include <iostream>
/* using std::cout; */
/* using std::endl; */
#include <dirent.h>
#include <string.h>

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <fstream>
#include <sstream>

using std::map;
using std::set;
using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;
using std::ostringstream;


DicProducer::DicProducer(std::string, SplitTool* tool) 
{

}
DicProducer::~DicProducer(){

}
void DicProducer::buildEnDict(){

}
void DicProducer::buildCnDict(){
    vector<string> words;
    string dir = Configuration::getInstance()->getConfigMap()["chinese_directory"];
    DIR * dirp = opendir(dir.data());
    if(nullptr == dirp){
        // todo
        perror("opendir failed");
        return;
    }
    struct dirent * entry;
    while((entry = readdir(dirp)) != nullptr){
        if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
            /* std::cout << entry->d_name << std::endl; */
            ifstream ifs(dir + entry->d_name);
            if(!ifs){
                std::cerr << "ifstream open file failed!" << std::endl;
                return;
            }
            


            ifs.close();
        }
    }// !while, 目录遍历完成
    std::cout << "sizeof(vector) = " << words.size() << std::endl;
    closedir(dirp);
    return;
}
void DicProducer::createIndex(){

}
void DicProducer::store(){

}

