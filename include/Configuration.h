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
    void chdir();
private:
    static Configuration * _pInstance;
    std::string _config_absolute_path;
    std::string _config_file_path;
    std::map<std::string,std::string> _configs;
};


#endif
