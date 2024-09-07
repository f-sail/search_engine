/* #include "Configuration.h" */

#include "../include/Configuration.h"

#include <fstream>

using std::map;
using std::string;
using std::ifstream;

Configuration * Configuration::_pInstance = nullptr;

Configuration::Configuration()
: _configFilePath("../conf/myconf.conf")
{
    ifstream ifs(_configFilePath);
    /* 读配置文件，并把键值对存到map里 */
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

