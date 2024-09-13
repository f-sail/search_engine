#ifndef __LRU_CHCHE_H__
#define __LRU_CHCHE_H__

#include <unordered_map>
#include <string>
#include <mutex>

class LRUCache{
protected:
    struct KeyValue{
        friend LRUCache;
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
public:
    LRUCache(size_t capacity);
    ~LRUCache(void);
    std::string get(const std::string& key);
    void put(const std::string& key, const std::string& value);
private:
    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;
    void enqueue(const std::string& key, const std::string& value);
    void dequeue(void); 
    void refresh(KeyValue*);
private:
    size_t _capacity;
    KeyValue* _phead;
    KeyValue* _ptail;
    std::unordered_map<std::string, KeyValue*> _kvPtrs;
    std::mutex _mutex;
    std::lock_guard<std::mutex> _lock;
};

#endif
