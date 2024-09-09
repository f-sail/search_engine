#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include <string>
#include <vector>

class DirScanner{
public:
    DirScanner();
    ~DirScanner() = default;
    std::vector<std::string>& getFiles();
    void traverse(std::string dir);
private:
    std::vector<std::string> _files;
};


#endif
