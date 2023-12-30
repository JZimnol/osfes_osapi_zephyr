#include "CUnit.h"
#include "osapi.h"

static volatile int testFlag;
static volatile int runFlag = 1;

class TestThread07 : public osapi::Thread {
    public:
	TestThread07() : Thread(3, 256, osapi::JOINABLE)
	{
		;
	}

    private:
	virtual void job()
	{
		testFlag = 1;
		while (runFlag) {
			sleep(5);
		}
	}
};

void TEST_ThreadRunDetailed(void)
{
	printf("Running test 07: detailed thread run             ");

	// set initial test flag value
	testFlag = 0;
	// create thread
	TestThread07 t;
	// thread should not be running at this point
	CU_ASSERT(false == t.isRunning());

	// get number of threads from Zephyr
	// unsigned int taskCount = NO FUNCTION AVAILABLE!

	// run the thread
	CU_ASSERT(true == t.run());

	// this should increase thread count in Zephyr
	// CU_ASSERT(taskCount+1 == NO FUNCTION AVAILABLE!);

	// it shouldn't be possible to run the thread again
	CU_ASSERT(false == t.run());
	// wait a while
	k_msleep(10);
	// test result
	CU_ASSERT(1 == testFlag);
	runFlag = 0;
	CU_ASSERT(true == t.join(100));
	// thread should not be running at this point
	CU_ASSERT(false == t.isRunning());
}
