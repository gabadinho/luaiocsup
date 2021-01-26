/* gtestMain.cpp */
/* Author:  Marty Kraimer Date:    17MAR2000 */

#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "epicsExit.h"
#include "epicsThread.h"
#include "iocsh.h"
#include "initHooks.h"

#include <gtest/gtest.h>
#include "lis_defs.h"

TEST(module_name, test_name) {
    std::cout << "Hello world!" << std::endl;
    // Google Test will also provide macros for assertions.
    ASSERT_EQ(1+1, 2);
}

void gtestInitHook(initHookState state) {
    if (state==initHookAtIocBuild) {
        printf("AAAAAAAAAA\n");
        ::testing::InitGoogleTest();
    } else if (state==initHookAfterIocBuilt) {
        printf("BBBBBBBBBBBBB\n");
        RUN_ALL_TESTS();
    }
}

int main(int argc,char *argv[])
{
    initHookRegister(gtestInitHook);
    if(argc>=2) {
        iocsh(argv[1]);
        epicsThreadSleep(.2);
    }
    iocsh(NULL);
    epicsExit(0);
    return(0);
}
