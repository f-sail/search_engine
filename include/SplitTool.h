#ifndef __SPLIT_TOOL__
#define __SPLIT_TOOL__

#include <string>
#include <vector>

namespace cppjieba{
    class Jieba;
};

struct WebPage{
    std::string docid;
    std::string url;
    std::string title;
    std::string content;
};

class SplitTool{
public:
    SplitTool() = default;
    virtual ~SplitTool() = 0;
    virtual std::vector<std::string> cut(std::string &) = 0;
    static size_t nBytesCode(const char ch);
    static WebPage rss(const std::string& doc);
};

class SplitToolCppJieba
: public SplitTool
{
public:
    SplitToolCppJieba();
    ~SplitToolCppJieba();
    SplitToolCppJieba(const SplitToolCppJieba&) = delete;
    SplitToolCppJieba& operator=(const SplitToolCppJieba&) = delete;
    std::vector<std::string> cut(std::string &) override;
private:
    cppjieba::Jieba* _pJieba;
};


#endif
