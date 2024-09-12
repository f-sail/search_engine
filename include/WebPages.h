#ifndef __WEB_PAGES_H__
#define __WEB_PAGES_H__

/* #include <cwchar> */
#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

class SplitTool;

class WebPages{
public:
    static WebPages* getInstance();
    std::vector<WebPages> doQuery(std::string key);
    WebPages(const WebPages&) = delete;
    WebPages& operator=(const WebPages&) = delete;
    ~WebPages();
private:
    WebPages();
    void Loading();
private:
    static WebPages* _pInstance;
    SplitTool* _cutter;
    std::ifstream _ifsPage;
    std::unordered_map<int, std::pair<int, int>> _offsetLib;
    std::unordered_map<std::string, std::unordered_map<int, double>> _indexLib;
};

#endif
