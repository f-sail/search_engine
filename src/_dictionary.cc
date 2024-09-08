#include "../include/Configuration.h"
#include "../include/SplitTool.h"
#include "../include/DicProducer.h"

#include <unistd.h>
#include <limits.h>
#include <string.h>

#include <iostream>
using std::cout;

int main(int args, char *argv[])
{
    system("pwd");
    Configuration::getInstance()->chdir();
    system("pwd");


    cout << ">> 语料读取中...\n";   
    DicProducer dp("", new SplitToolCppJieba());
    cout << ">> 已生成文件：/search_engine/data/dict.dat\n"; 
    cout << ">> 已生成文件：/search_engine/data/dictindex.dat\n";

    return 0;
}
