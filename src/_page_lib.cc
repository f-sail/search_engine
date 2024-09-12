#include "../include/_page_lib.h"

#include <iostream>
using std::cout;
using std::endl;

int main(int args, char *argv[])
{
    Configuration::getInstance()->chdir();
    PageLib pl;
    pl.create();
    pl.store();
    
    cout << "--------------\n";
    PageLibPreprocessor plp;
    plp.cutRedundantPage();
    plp.buildInvertIndexMap();
    /* plp.storeOnDisk(); */

    return 0;
}

