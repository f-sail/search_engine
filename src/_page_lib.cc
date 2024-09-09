#include "../include/Configuration.h"
#include "../include/DirScanner.h"
#include "../include/PageLib.h"

#include <iostream>
using std::cout;
using std::endl;

int main(int args, char *argv[])
{
    PageLib pl;
    pl.create();
    pl.store();

    return 0;
}

