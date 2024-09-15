#include "../include/KeyRecommander.h"
#include "../include/TcpConnection.h"
#include "../include/Dictionary.h"
#include "../include/Cache.h"

#include "../include/log4cpp.h"
#include "../include/json.hpp"

#include <iostream>
#include <algorithm>
#include <thread>
#include <vector>

#define RESP_NUM 10

using std::string;
using std::vector;

KeyRecommander::KeyRecommander(std::string key)
: _sought(key)
{
    return;
}

KeyRecommander::~KeyRecommander(){

}

std::string KeyRecommander::doQuery(){
    std::thread::id tid(std::this_thread::get_id());
    Cache* cache = CacheManager::getInstance()->getCache(tid);
    string ret(cache->get("kr_" + _sought));
    if(string() != ret){
        LOG_INFO("used cache");
        return ret;
    }

    vector<std::pair<string, int>> vec(Dictionary::getInstance()->doQuery(_sought));
    for(auto& pair: vec){
        CandidateResult cr;
        cr._word = pair.first;
        cr._freq = pair.second;
        cr._dist = distance(cr._word);
        _prique.push(cr);
    }
    int n = RESP_NUM;
    nlohmann::json json;
    while(_prique.size() && n > 0){
        string word = _prique.top()._word;
        if(json.count(word)){
        _prique.pop();
            continue;
        }
        json.emplace_back(std::move(word));
        /* std::cout << _prique.top()._dist << " "; */
        _prique.pop();
        --n;
    }
    cache->put("kr_" + _sought, json.dump());
    return json.dump();
}

int KeyRecommander::distance(std::string candiate){
    std::string& lhs = _sought;
    std::string& rhs = candiate;
    size_t m = _sought.length();
    size_t n = candiate.length();
    int matrix[m + 1][n + 1];
    for(size_t i = 0; i <= m; ++i){
        matrix[i][0] = i;
    }
    for(size_t j = 0; j <= n; ++j){
        matrix[0][j] = j;
    }
    // 动态填充矩阵
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (lhs[i - 1] == rhs[j - 1]) {
                matrix[i][j] = matrix[i - 1][j - 1];  // 如果字符相同，不需要操作
            } else {
                matrix[i][j] = 1 + std::min({matrix[i - 1][j],  // 删除
                                         matrix[i][j - 1],      // 插入
                                         matrix[i - 1][j - 1]});// 替换
            }
        }
    }
    return matrix[m][n];
}
