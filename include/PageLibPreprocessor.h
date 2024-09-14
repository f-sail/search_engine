#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__

#include <map>
#include <pthread.h>
#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

namespace simhash{
class Simhasher;
}

class SplitTool;

class PageLibPreprocessor{
    struct WordStatistics{
        int DF;
        std::unordered_map<int, int> docTF; // docTF<docid, TF>
    };
public:
    PageLibPreprocessor();
    ~PageLibPreprocessor();
    PageLibPreprocessor(const PageLibPreprocessor&) = delete;
    PageLibPreprocessor& operator=(const PageLibPreprocessor&) = delete;
    void cutRedundantPage();
    void buildInvertIndexMap();
    void storeOnDisk();
private:
    void loadOffset();
    bool isSimilar(uint64_t);
    void wordStatistics(int, std::string);
    void analyze(const std::string &, std::ofstream&, simhash::Simhasher&);
    void normalization();
private:
    std::vector<uint64_t> _pageList;
    std::map<int, std::pair<int, int>> _offsetLib;
    std::unordered_map<int, std::pair<int, int>> _new_offsetLib;
    std::unordered_map<std::string,std::set<std::pair<int, double>>> _invertIndexLib;
    std::unordered_map<std::string, WordStatistics> _wordsInfo;
    SplitTool* _wordCutter;
};

#endif
