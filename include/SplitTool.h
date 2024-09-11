#ifndef __SPLIT_TOOL__
#define __SPLIT_TOOL__

#include <string>
#include <vector>

namespace cppjieba{
    class Jieba;
};

class SplitTool{
public:
    SplitTool() = default;
    virtual ~SplitTool() = 0;
    virtual std::vector<std::string> cut(std::string &) = 0;
    static size_t nBytesCode(const char ch);
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
