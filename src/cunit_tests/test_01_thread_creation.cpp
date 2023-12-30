#include "CUnit.h"
#include "osapi.h"
#include <string.h>

static volatile int testFlag;

class TestThread1 : public osapi::Thread {
    public:
	TestThread1() : Thread(3, 256, osapi::NOT_JOINABLE)
	{
		;
	}

    private:
	virtual void job()
	{
		testFlag = 1;
	}
};

void TEST_ThreadCreation(void)
{
	printf("Running test 01: thread creation                 ");

	// set initial test flag value
	testFlag = 0;
	// create thread
	TestThread1 t;
	// run the thread
	CU_ASSERT(true == t.run());
	// wait a whiles
	k_msleep(1000);
	// test result
	CU_ASSERT(1 == testFlag);
}
