#ifndef __DICTRODUCER_H__
#define __DICTRODUCER_H__

#include <map>
#include <set>
#include <string>
#include <vector>

class SplitTool;

class DicProducer{
public:
    DicProducer(std::string, SplitTool* tool);
    DicProducer(const DicProducer&) = delete;
    DicProducer &operator=(const DicProducer&) = delete;
    ~DicProducer();
    void buildFreq();
    void buildDict();
    void createIndex();
    void store();
private:
    //void getFiles(const std::string&);
    size_t nBytesCode(const char ch);
private:
    std::vector<std::string> _files;
    std::vector<std::pair<std::string, int>> _dict;
    std::map<std::string, int> _mFreq;
    std::map<std::string, std::set<int>> _index;
    SplitTool* _cuttor;
};

#endif
