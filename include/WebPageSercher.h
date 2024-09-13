#ifndef __WEBPAGESERCHER_H__
#define __WEBPAGESERCHER_H__

#include "WebPageQuery.h"

#include <queue>
#include <string>

struct CosineValue;

class WebPageSercher{
public:
    WebPageSercher(const std::string& key);
    ~WebPageSercher();
    std::string doQuery();
private:
    void buildPriorityQueue();
private:
    std::string _sought;
    WebPageQuery::IndexTable _table;
    std::priority_queue<CosineValue> _pq;
};



#endif
