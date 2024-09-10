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
    Configuration::getInstance()->chdir();
    DicProducer dp("", new SplitToolCppJieba());

    return 0;
}
