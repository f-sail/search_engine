#include "../include/Configuration.h"
#include "../include/DirScanner.h"
#include "../include/PageLib.h"
#include "../include/PageLibPreprocessor.h"

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

