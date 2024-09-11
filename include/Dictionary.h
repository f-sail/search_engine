#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <map>
#include <set>
#include <string>
#include <vector>

class Dictionary{
public:
    static Dictionary * getInstance();
    std::vector<std::pair<std::string, int>> doQuery(const std::string& key);
    Dictionary(const Dictionary&) = delete;
    Dictionary &operator=(const Dictionary&) = delete;
private:
    Dictionary();
    ~Dictionary();
    void Loading();
    /* int distance(std::string candidate); */
private:
    static Dictionary* _pInstance;
    std::vector<std::pair<std::string, int>> _dict;
    std::map<std::string, std::set<int>> _index;
};

#endif
