#include "../include/Cache.h"
#include "../include/log4cpp.h"
#include "../include/Configuration.h"

#include <features.h>

#include <cstdlib>
#include <iostream>
#include <mutex>

using std::cout;
using std::endl;
using std::mutex;
using std::string;
using std::vector;
using std::lock_guard;


/* ================ KeyValue ================ */
KeyValue::KeyValue(const std::string& key, const std::string& value)
: _key(key)
, _value(value)
{
    return;
}

KeyValue::~KeyValue(void){}

//KeyValue* LRU::KeyValue::ptr(void){
//    return this;
//}


/* ================ CacheManager ================ */
LRU::LRU(void)
: _capacity(stoul(Configuration::getInstance()->getConfigMap()["capacity_LRU"]))
, _phead(new KeyValue("", ""))
, _ptail(new KeyValue("", ""))
{
    _phead->_prev = nullptr;
    _phead->_next = _ptail;
    _ptail->_prev = _phead;
    _ptail->_next = nullptr;

    return;
}

LRU::~LRU(void){
    if(_phead){
        KeyValue *pcurr = _phead;
        while(pcurr){
            _phead = _phead->_next;
            delete pcurr;
            pcurr = _phead;
        }
    _phead = nullptr;
    _ptail = nullptr;
    }
    return;
}

std::string LRU::get(const std::string& key){
    if(_kvPtrs.count(key)){
        refresh(_kvPtrs[key]);
        return _kvPtrs[key]->_value;
    }
    static string nullstr = string();
    return nullstr;
}

void LRU::put(const std::string& key, const std::string& value){
    if(_kvPtrs.count(key)){
        _kvPtrs[key]->_value = value;
        refresh(_kvPtrs[key]);
    }else if(_kvPtrs.size() < _capacity){
        enqueue(key, value);
    }else{
        dequeue();
        enqueue(key, value);
    }
    return;
}

std::vector<std::string> LRU::getKeys()const{
    vector<string> vec;
    /* 倒序插入 */
    KeyValue* pcurr = _ptail->_prev;
    while(pcurr != _phead){
        vec.push_back(pcurr->_key);
        pcurr = pcurr->_prev;
    }
    return vec;
}

void LRU::enqueue(const std::string& key, const std::string& value){
    KeyValue* p_kv = new KeyValue(key, value);
    p_kv->_prev = _phead;
    p_kv->_next = _phead->_next;
    _phead->_next = p_kv;
    p_kv->_next->_prev = p_kv;
    _kvPtrs[key] = p_kv;
    return;
}

void LRU::dequeue(void){
    KeyValue* p_kv = _ptail->_prev;
    p_kv->_prev->_next = _ptail;
    _ptail->_prev = p_kv->_prev;
    _kvPtrs.erase(p_kv->_key);
    delete p_kv;
    return;
}

void LRU::refresh(KeyValue* p_kv){
    if(p_kv == _phead->_next){
        return;
    }
    p_kv->_prev->_next = p_kv->_next;
    p_kv->_next->_prev = p_kv->_prev;

    p_kv->_prev = _phead;
    p_kv->_next = _phead->_next;
    
    _phead->_next = p_kv;
    p_kv->_next->_prev = p_kv;
    return;
}


/* ================ Cache ================ */
Cache::Cache()
: _cache(new LRU())
, _update(new LRU())
, _pmutex(new mutex())
{
    return;
}

Cache::~Cache(){
    delete _cache;
    _cache = nullptr;
    delete _update;
    _update = nullptr;
    delete _pmutex;
    _pmutex = nullptr;
}

std::string Cache::get(const std::string& key)const{
    lock_guard<mutex> lock(*_pmutex);
    _update->get(key);
    return _cache->get(key);
}
void Cache::put(const std::string& key, const std::string& value){
    lock_guard<mutex> lock(*_pmutex);
    _update->put(key, string(""));
    return _cache->put(key, value);
}
std::vector<std::string> Cache::getUpdateList(void)const{
    lock_guard<mutex> lock(*_pmutex);
    return _update->getKeys(); 
}

void Cache::update(const Cache* rhs){
    vector<string> updateList = rhs->getUpdateList();
    lock_guard<mutex> lock(*_pmutex);
    for(const string& key: updateList){
        put(key, rhs->get(key));
    }
    return;
}

/* ================ CacheManager ================ */

CacheManager* CacheManager::_pInstance = nullptr;

CacheManager::CacheManager()
: _common(new Cache())
{
    return;
}

CacheManager::~CacheManager(){
    if(_common){
        delete _common;
        _common = nullptr;
    }
    for(auto pair: _locals){
        if(pair.second){
            delete pair.second;
            pair.second = nullptr;
        }
    }
    if(_pInstance){
        delete _pInstance;
        _pInstance = nullptr;
    }
    return;
}

CacheManager* CacheManager::getInstance(){
    if(!_pInstance){
        _pInstance = new CacheManager();
    }
    return _pInstance;
}

void CacheManager::updateCache(){
    LOG_INFO("CacheManager::updateCache()");
    for(auto& pair: _locals){
        _common->update(pair.second);
    }
    for(auto& pair: _locals){
        pair.second->update(_common);
    }
    return;
}

Cache* CacheManager::getCache(std::thread::id id){
    return _locals[id];
}

void CacheManager::setCache(std::thread::id id, Cache* cache){
    _locals.emplace(id, std::move(cache));
    return;
}

