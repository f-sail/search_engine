#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__


class PageLibPreprocessor{
public:
    PageLibPreprocessor() = default;
    ~PageLibPreprocessor() = default;
    void cutRedundantPage();
    void buildInvertIndexMap();
    void storeOnDisk();

private:

};

#endif
