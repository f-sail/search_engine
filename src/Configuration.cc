/* #include "Configuration.h" */

#include "../include/Configuration.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include <iostream>
#include <fstream>

using std::map;
using std::string;
using std::ifstream;

Configuration * Configuration::_pInstance = nullptr;

Configuration::Configuration()
: _config_file_path("../conf/myconf.conf")
{
    chdir();
    ifstream ifs(_config_file_path);
    /* 读配置文件，并把键值对存到map里 */
    if(!ifs){                                                  
        perror("ifstream open file failed!");
        return;
    }

    string line;
    while(getline(ifs,line)){
        size_t pos = string::npos;
        if((pos = line.find("=")) != string::npos){
            _configs[line.substr(0,pos)] = line.substr(pos + 1, line.size() - pos - 1);
        }
    }
    ifs.close();
    return;
}
Configuration::~Configuration(){
    if(_pInstance){
        delete _pInstance;
    }
    return;
}
Configuration * Configuration::getInstance(){
    if(nullptr == _pInstance){
        _pInstance = new Configuration();
    }
    return _pInstance;
}
std::map<std::string,std::string>& Configuration::getConfigMap(){
    return _configs;
}
void Configuration::chdir(){
    /* 改变工作目录 */
    char path[1024] = {0};
    size_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    for(int i = strlen(path) - 1; i >= 0; --i){
        if('/' == path[i]){
            path[i] = '\0';
            break;
        }
    }
    /* printf("path = %s\n", path); */
    if(::chdir(path) != 0){
        std::cerr << "无法改变工作目录\n";
        return;
    } 
}
