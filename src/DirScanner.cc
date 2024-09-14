#include "../include/DirScanner.h"
#include "../include/Configuration.h"

#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

DirScanner::DirScanner(){}

std::vector<std::string>& DirScanner::getFiles(){
    return _files;
}
void DirScanner::traverse(std::string dir){
    DIR * dirp = opendir(dir.data());
    if(nullptr == dirp){
        perror("opendir failed");
        return;
    }
    struct dirent * entry;                                            
    while((entry = readdir(dirp)) != nullptr){                        
        if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){
            if(entry->d_type == DT_DIR){ 
                traverse(dir + entry->d_name + '/');
            }else if(entry->d_type == DT_REG){
                _files.push_back(dir + entry->d_name);
            }
        }
    }

    return;
}

