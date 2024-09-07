#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <map>

class Configuration{
public:
    Configuration();
    ~Configuration();
    static Configuration * getInstance();
    std::map<std::string,std::string>& getConfigMap();
private:
    static Configuration * _pInstance;
    std::string _configFilePath;
    std::map<std::string,std::string> _configs;
};


#endif
