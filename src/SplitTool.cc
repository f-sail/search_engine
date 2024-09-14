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


/* SplitTool */
SplitTool::~SplitTool(){}

size_t SplitTool::nBytesCode(const char ch){
    if(ch & (1 << 7)){
        int nBytes = 1;
        for(int idx = 0; idx != 6; ++idx){
            if(ch & (1 << (6 - idx))){
                ++nBytes;
            }else{
                break;
            }
        }
        return nBytes;
    }
    return 1;
}

WebPage SplitTool::rss(const std::string& doc){
    size_t pos_docid_begin = doc.find("<docid>") + strlen("<docid>");
    size_t pos_docid_end = doc.find("</docid>");                                          
    
    size_t pos_url_begin = doc.find("<url>", pos_docid_end) + strlen("<url>");
    size_t pos_url_end = doc.find("</url>");
    
    size_t pos_title_begin = doc.find("<title>", pos_url_end) + strlen("<title>");
    size_t pos_title_end = doc.find("</title>");
    
    size_t pos_content_begin = doc.find("<content>", pos_title_end) + strlen("<content>");
    size_t pos_content_end = doc.find("</content>");
    
    WebPage wp;
    wp.docid = doc.substr(pos_docid_begin, pos_docid_end - pos_docid_begin);
    wp.url = doc.substr(pos_url_begin, pos_url_end - pos_url_begin);
    wp.title = doc.substr(pos_title_begin, pos_title_end - pos_title_begin);
    wp.content = doc.substr(pos_content_begin, pos_content_end - pos_content_begin);

    return wp;
}

/* SplitToolCppJieba */
SplitToolCppJieba::SplitToolCppJieba()
: _pJieba(nullptr)
{
    const char* const DICT_PATH = "../include/cppjieba/dict/jieba.dict.utf8";    
    const char* const HMM_PATH = "../include/cppjieba/dict/hmm_model.utf8";
    const char* const USER_DICT_PATH = "../include/cppjieba/dict/user.dict.utf8";
    const char* const IDF_PATH = "../include/cppjieba/dict/idf.utf8";
    const char* const STOP_WORD_PATH = "../include/cppjieba/dict/stop_words.utf8";
    _pJieba = new cppjieba::Jieba(DICT_PATH, HMM_PATH, DICT_PATH, IDF_PATH, STOP_WORD_PATH);
    return;
}

SplitToolCppJieba::~SplitToolCppJieba(){
    if(_pJieba){
        delete _pJieba;
        _pJieba = nullptr;
    }
    return;
}

vector<string> SplitToolCppJieba::cut(string & str){
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
    _pJieba->Cut(str, words);

    return words;
}

