#ifndef __SPLIT_TOOL__
#define __SPLIT_TOOL__

#include <string>
#include <vector>

class SplitTool{
public:
    SplitTool();
    virtual ~SplitTool() = 0;
    virtual std::vector<std::string> cut() = 0;
};


class SplitToolCppJieba
: public SplitTool
{
    public:
    SplitToolCppJieba();
    ~SplitToolCppJieba();
    std::vector<std::string> cut() override;
};


#endif
