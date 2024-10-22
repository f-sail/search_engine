#ifndef __LRU_CHCHE_H__
#define __LRU_CHCHE_H__

#include <unordered_map>
#include <string>
#include <thread>
#include <vector>
#include <mutex>


class LRU;
class Cache;
class CacheManager;
class ThreadPool;

/* ================ KeyValue ================ */
struct KeyValue{
    friend LRU;
    friend Cache;
public:
    KeyValue(const std::string& key, const std::string& value);
    ~KeyValue(void);
    /* KeyValue* ptr(void); */
private:
    std::string _key;
    std::string _value;
    KeyValue* _prev;
    KeyValue* _next;
};

/* ================ LRU ================ */
class LRU{
    friend Cache;
public:
    LRU(void);
    ~LRU(void);
    std::string get(const std::string& key);
    void put(const std::string& key, const std::string& value);
    /* void update(const LRU*); */
    std::vector<std::string> getKeys()const;
private:
    LRU(const LRU&) = delete;
    LRU& operator=(const LRU&) = delete;
    void enqueue(const std::string& key, const std::string& value);
    void dequeue(void); 
    void refresh(KeyValue*);
private:
    size_t _capacity;
    KeyValue* _phead;
    KeyValue* _ptail;
    std::unordered_map<std::string, KeyValue*> _kvPtrs;
};


/* ================ Cache ================ */
class Cache{
    friend CacheManager;
public:
    Cache();
    ~Cache();
    std::string get(const std::string& key);
    void put(const std::string& key, const std::string& value);
    void updateToCommon(Cache*);
    void updateByCommon(Cache*);
    void swapLRU();
private:
    LRU* _cache;
    LRU* _update;
    std::recursive_mutex _mutex;
};


/* ================ CacheManager ================ */
class CacheManager{
    friend ThreadPool;
public:
    ~CacheManager();
    void updateCache();
    Cache* getCache(std::thread::id);
    static CacheManager* getInstance();
private:
    CacheManager();
    void setCache(std::thread::id, Cache*);
    CacheManager(const CacheManager&) = delete;
    CacheManager& operator=(const CacheManager&) = delete;
private:
    Cache* _common;
    static CacheManager* _pInstance;
    std::unordered_map<std::thread::id, Cache*> _locals;
};

#endif
