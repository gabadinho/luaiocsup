#include <cstdio>

#include <epicsStdio.h>
#include <epicsExport.h>
#include <initHooks.h>
#include <epicsExit.h>

#include <gtest/gtest.h>

void gtestHook(initHookState state) {
    static int testrun_res = 0;

    if (state == initHookAtIocBuild) {
        epicsStdoutPrintf("gtestIOC: initializing GoogleTest\n");
        ::testing::InitGoogleTest();
    } else if (state == initHookAfterDatabaseRunning) {
        epicsStdoutPrintf("gtestIOC: running GoogleTest tests\n");
        testrun_res = RUN_ALL_TESTS();
    } else if (state == initHookAfterIocRunning) {
        epicsStdoutPrintf("gtestIOC: exiting...\n");
        epicsExit(testrun_res);
    }
}

static void gtestRegister(void) {
    initHookRegister(gtestHook);
}

extern "C" {
    epicsExportRegistrar(gtestRegister);
}

