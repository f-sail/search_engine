#ifndef __PAGELIB_H__
#define __PAGELIB_H__

#include <vector>
#include <string>
#include <map>

class DirScanner;

class PageLib{
    struct RSSIteam{
        std::string title;
        std::string url;
        std::string content;
    };
public:
    PageLib();
    ~PageLib();
    void create();
    void store();
private:
    void read(const std::string& filepath);
private:
    DirScanner* _pDirScanner;
    std::vector<RSSIteam> _pages;
    std::map<int, std::pair<int,int>> _offsetLib;
};

#endif
