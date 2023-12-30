#include <zephyr/kernel.h>
#include "CUnit.h"
#include "Basic.h"

extern void TEST_ThreadName(void);
extern void TEST_ThreadCreation(void);
extern void TEST_ThreadJoin(void);
extern void TEST_ThreadSuspendResume(void);
extern void TEST_ThreadSleep(void);
extern void TEST_ThreadPriority(void);
extern void TEST_Mutex(void);
extern void TEST_RecursiveMutex(void);
extern void TEST_MortalThread(void);
extern void TEST_ThreadPriorityChange(void);
extern void TEST_ThreadRunDetailed(void);
extern void TEST_ThreadStateChecking(void);
extern void TEST_ThreadSuspendInactive(void);
extern void TEST_ThreadStackSize(void);

#define SLEEP_TIME_MS 250

int main(void)
{
	printf("\n*** Running tests on \"%s\" ***\n", CONFIG_BOARD);
	// initialize CUnit registry
	CU_initialize_registry();

	// create new test suite for testing thread API
	CU_pSuite threadTestSuite = CU_add_suite("osapi::thread", NULL, NULL);

	// add tests to the suite
	CU_add_test(threadTestSuite, "Thread name             ", TEST_ThreadName);
	CU_add_test(threadTestSuite, "Thread creation         ", TEST_ThreadCreation);
	CU_add_test(threadTestSuite, "Thread join             ", TEST_ThreadJoin);
	CU_add_test(threadTestSuite, "Thread suspend          ", TEST_ThreadSuspendResume);
	CU_add_test(threadTestSuite, "Thread delay            ", TEST_ThreadSleep);
	CU_add_test(threadTestSuite, "Thread priority         ", TEST_ThreadPriority);

	// create new test suite for additional tests
	CU_pSuite detailedTestSuite = CU_add_suite("osapi::thread additional", NULL, NULL);
	CU_add_test(detailedTestSuite, "Priority change         ", TEST_ThreadPriorityChange);
	CU_add_test(detailedTestSuite, "Run - detailed          ", TEST_ThreadRunDetailed);
	CU_add_test(detailedTestSuite, "Thread state            ", TEST_ThreadStateChecking);
	CU_add_test(detailedTestSuite, "Thread suspend inactive ", TEST_ThreadSuspendInactive);
	CU_add_test(detailedTestSuite, "Thread stack size       ", TEST_ThreadStackSize);

	// create new test suite for testing mutex API and add tests to the suite
	CU_pSuite mutexTestSuite = CU_add_suite("osapi::mutex", NULL, NULL);
	CU_add_test(mutexTestSuite, "Mutex                   ", TEST_Mutex);
	CU_add_test(mutexTestSuite, "RecursiveMutex          ", TEST_RecursiveMutex);

	// create new test suite for testing mortal thread API and add tests to the suite
	CU_pSuite mortalTestSuite = CU_add_suite("osapi::mortal thread", NULL, NULL);
	CU_add_test(mortalTestSuite, "Thread kill             ", TEST_MortalThread);

	// set mode to verbose
	CU_basic_set_mode(CU_BRM_VERBOSE);
	// run all tests
	CU_basic_run_tests();

	// print results
	printk("\n");
	CU_basic_show_failures(CU_get_failure_list());

	// deinitialize
	CU_cleanup_registry();

	printk("Done testing.\n");

	return 0;
}
