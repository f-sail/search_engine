/* #include "SplitTool.h" */
/* #include "cppjieba/Jieba.hpp" */

#include "../include/SplitTool.h"
#include "../include/cppjieba/Jieba.hpp"
#include "../include/Configuration.h"

#include <dirent.h>

#include <cstdlib>
#include <fstream>
#include <sstream>

/* #include <filesystem> */
/* using std::filesystem; */

using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;
using std::ostringstream;



SplitTool::~SplitTool(){

}

SplitToolCppJieba::~SplitToolCppJieba(){

}

vector<string> SplitToolCppJieba::cut(string & str){
    const char* const DICT_PATH = "../include/cppjieba/dict/jieba.dict.utf8";    
    const char* const HMM_PATH = "../include/cppjieba/dict/hmm_model.utf8";
    const char* const USER_DICT_PATH = "../include/cppjieba/dict/user.dict.utf8";
    const char* const IDF_PATH = "../include/cppjieba/dict/idf.utf8";
    const char* const STOP_WORD_PATH = "../include/cppjieba/dict/stop_words.utf8";

    cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, DICT_PATH, IDF_PATH, STOP_WORD_PATH);

    /* 清洗 */
    auto it = str.begin();
    while(it != str.end()){
        if(0 == (*it & 0x80)){
            if(!isalpha(*it)){
                *it = ' ';
            }else{
                *it = tolower(*it);
            }
        }
        ++it;
    }                                        
    
    /* 分词 */
    vector<string> words; 
    jieba.Cut(str, words);

    return words;
}
