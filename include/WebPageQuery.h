#ifndef __WEB_PAGE_QUERY_H__
#define __WEB_PAGE_QUERY_H__

/* #include <cwchar> */
#include <fstream>
#include <set>
#include <string>
#include <vector>
/* #include <fstream> */
#include <unordered_map>
/* #include <unordered_set> */

class SplitTool;

class WebPageQuery{
public:
    using IndexTable = std::unordered_map<std::string, std::unordered_map<int, double>>;
public:
    static WebPageQuery* getInstance();
    
    IndexTable doQuery(std::string key);
    std::string getPage(int docid);
    
    WebPageQuery(const WebPageQuery&) = delete;
    WebPageQuery& operator=(const WebPageQuery&) = delete;
    ~WebPageQuery();
private:
    WebPageQuery();
    void Loading();
    std::set<int> getDocids(const std::set<std::string>&);
private:
    static WebPageQuery* _pInstance;
    SplitTool* _cutter;
    IndexTable _indexLib;
    std::ifstream _ifs;
    std::unordered_map<int, std::pair<int, int>> _offsetLib;
};

#endif
