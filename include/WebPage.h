#ifndef __WEB_PAGE_H__
#define __WEB_PAGE_H__


/* #include "Configuration.h" */
#include <unordered_map>
#include <string>
#include <set>

class SplitTool;

class WebPage{
public:
    WebPage();
    ~WebPage();
private:
    void Loading();
private:
    std::unordered_map<int, std::pair<int, int>> _offsetLib;
    std::unordered_map<std::string, std::pair<int, double>> _indexLib;
    SplitTool* _cutter;
};

#endif
