#include "../include/WebPageSercher.h"
#include "../include/json.hpp"
#include "../include/log4cpp.h"
#include "../include/Cache.h"

#include <cmath>

#include <queue>
#include <iostream>
#include <unordered_map>

using std::cout;
using std::endl;
using std::string;
using std::unordered_map;


struct CosineValue{
public:
    int _docid;
    double _value;
public:
    CosineValue(int docid = 0, double value = 0):_docid(docid),_value(value){}
    bool operator<(const CosineValue & rhs)const{return _value < rhs._value;}
};


WebPageSercher::WebPageSercher(const std::string& key)
: _sought(key)
, _table(WebPageQuery::getInstance()->doQuery(_sought))
{
    buildPriorityQueue();
    return;
}

WebPageSercher::~WebPageSercher(){}

std::string WebPageSercher::doQuery(){
    /* 查缓存 */
    std::thread::id tid(std::this_thread::get_id());
    Cache* cache = CacheManager::getInstance()->getCache(tid);
    string ret(cache->get("wps_" + _sought));
    if(string("") != ret){
        /* LOG_INFO("used cache"); */
        puts(">> used cache");
        return ret;
    }
    puts(">> not used cache");

    nlohmann::json jsonArr;
    while(_pq.size()){
        string doc(WebPageQuery::getInstance()->getPage(_pq.top()._docid));
        _pq.pop();
        jsonArr.emplace_back(doc); 
    }
    cache->put("wps_" + _sought, jsonArr.dump());   // 存入缓存
    return jsonArr.dump();
}

void WebPageSercher::buildPriorityQueue(){
    unordered_map<int, double> dotProductSelf;
    
    for(std::pair<string, unordered_map<int, double>> pair: _table){
        for(const std::pair<int, double>& p :pair.second){
            if(dotProductSelf.count(p.first)){
                dotProductSelf[p.first] += std::pow(p.second, 2);
            }else{
                dotProductSelf[p.first] = std::pow(p.second, 2);
            }
        }
    }
    
    for(const auto& p: dotProductSelf){
        if(0 == p.first){
            continue;
        }
        double dotProduct = 0;
        for(std::pair<string, unordered_map<int, double>> pair: _table){
            dotProduct += pair.second[p.first] * pair.second[0];
        }
        _pq.emplace(p.first, dotProduct);
    }

    return;
}
