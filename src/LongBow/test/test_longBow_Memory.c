/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center (PARC)
 * All rights reserved.
 *  
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution. 
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *  
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @author Scott, Glenn <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox) and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include "../private/longBow_Memory.c"

#include <LongBow/unit-test.h>
#include <LongBow/debugging.h>

LONGBOW_TEST_RUNNER(test_longBow_Memory)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(test_longBow_Memory)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(test_longBow_Memory)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, longBowMemory_Allocate);
    LONGBOW_RUN_TEST_CASE(Global, longBowMemory_Reallocate);
    LONGBOW_RUN_TEST_CASE(Global, longBowMemory_StringCopy);
}

static uint64_t _setupAllocations;

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    _setupAllocations = longBowMemory_OutstandingAllocations();
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    if (longBowMemory_OutstandingAllocations() != _setupAllocations) {
        printf("%s Memory leak\n", longBowTestCase_GetFullName(testCase));
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, longBowMemory_Allocate)
{
    void *memory = longBowMemory_Allocate(10);
    assertNotNull(memory, "Return value from longBowMemory_Allocate(10) cannot be NULL.");
    longBowMemory_Deallocate((void **) &memory);
    assertNull(memory, "longBowMemory_Deallocated must NULL the pointer.");
}

LONGBOW_TEST_CASE(Global, longBowMemory_Reallocate)
{
    void *memory = longBowMemory_Allocate(10);
    assertNotNull(memory, "Return value from longBowMemory_Allocate(10) cannot be NULL.");
    
    memory = longBowMemory_Reallocate(memory, 100);
    assertNotNull(memory, "Return value from longBowMemory_Reallocate cannot be NULL.");
    
    longBowMemory_Deallocate((void **) &memory);
    assertNull(memory, "longBowMemory_Deallocated must NULL the pointer.");
}

LONGBOW_TEST_CASE(Global, longBowMemory_Reallocate_NULL)
{
    void *memory = longBowMemory_Reallocate(NULL, 100);
    assertNotNull(memory, "Return value from longBowMemory_Reallocate cannot be NULL.");
    
    longBowMemory_Deallocate((void **) &memory);
    assertNull(memory, "longBowMemory_Deallocated must NULL the pointer.");
}

LONGBOW_TEST_CASE(Global, longBowMemory_StringCopy)
{
    char *expected = "Hello World";

    char *actual = longBowMemory_StringCopy(expected);
    assertTrue(strcmp(expected, actual) == 0, "Expected '%s', actual '%s'", expected, actual);
    longBowMemory_Deallocate((void **) &actual);
}

int
main(int argc, char *argv[])
{
    LongBowTestRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(test_longBow_Memory);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
