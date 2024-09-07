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
    ~DicProducer();
    void buildEnDict();
    void buildCnDict();
    void createIndex();
    void store();
private:
    std::vector<std::string> _files;
    std::vector<std::pair<std::string, int>> _dict;
    std::map<std::string, std::set<int>> _index;
    SplitTool* _cuttor;
};

#endif
