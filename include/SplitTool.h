#ifndef __SPLIT_TOOL__
#define __SPLIT_TOOL__

#include <string>
#include <vector>

class SplitTool{
public:
    SplitTool() = default;
    virtual ~SplitTool() = 0;
    virtual std::vector<std::string> cut(std::string &) = 0;
};


class SplitToolCppJieba
: public SplitTool
{
    public:
    SplitToolCppJieba() = default;
    ~SplitToolCppJieba();
    std::vector<std::string> cut(std::string &) override;
};


#endif
