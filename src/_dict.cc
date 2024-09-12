#include "../include/_dict.h"

int main(int args, char *argv[]){
    Configuration::getInstance()->chdir();
    DicProducer dp("", new SplitToolCppJieba());

    return 0;
}
