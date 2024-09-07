#include "../include/SplitTool.h"

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <string>
using std::string;

int main(int args, char *argv[])
{
    SplitToolCppJieba cj;
    string str = "日本的明治维新比不上英国的光荣革命，但优于俄国的农奴解放和清朝的百日维新，";
    vector<string> vec(cj.cut(str));
    for(auto i: vec){
        cout << i << endl;
    }

    return 0;
}

