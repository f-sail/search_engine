#include "../include/LRUCache.h"

#include <features.h>
#include <mutex>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

LRUCache::KeyValue::KeyValue(const std::string& key, const std::string& value)
: _key(key)
, _value(value)
{return;}

LRUCache::KeyValue::~KeyValue(void){}

//LRUCache::KeyValue* LRUCache::KeyValue::ptr(void){
//    return this;
//}

LRUCache::LRUCache(size_t capacity)
: _capacity(capacity)
, _phead(new KeyValue(0, 0))
, _ptail(new KeyValue(0, 0))
, _mutex()
, _lock(_mutex)
{
    _phead->_prev = nullptr;
    _phead->_next = _ptail;
    _ptail->_prev = _phead;
    _ptail->_next = nullptr;

    return;
}

LRUCache::~LRUCache(void){
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

std::string LRUCache::get(const std::string& key){
    if(_kvPtrs.count(key)){
        refresh(_kvPtrs[key]);
        return _kvPtrs[key]->_value;
    }
    static string nullstr = string();
    return nullstr;
}

void LRUCache::put(const std::string& key, const std::string& value){
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

void LRUCache::enqueue(const std::string& key, const std::string& value){
    KeyValue* p_kv = new KeyValue(key, value);
    p_kv->_prev = _phead;
    p_kv->_next = _phead->_next;
    _phead->_next = p_kv;
    p_kv->_next->_prev = p_kv;
    _kvPtrs[key] = p_kv;
    return;
}

void LRUCache::dequeue(void){
    KeyValue* p_kv = _ptail->_prev;
    p_kv->_prev->_next = _ptail;
    _ptail->_prev = p_kv->_prev;
    _kvPtrs.erase(p_kv->_key);
    delete p_kv;
    return;
}

void LRUCache::refresh(KeyValue* p_kv){
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
