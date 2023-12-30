#include "CUnit.h"
#include "osapi.h"

static volatile int runFlag = 1;

class TestThread10 : public osapi::Thread {
    public:
	TestThread10() : Thread(2, 256, osapi::JOINABLE)
	{
		;
	}

    private:
	virtual void job()
	{
		while (runFlag) {
			sleep(5);
		}
	}
};

void TEST_ThreadStackSize(void)
{
	printf("Running test 10: thread stack size               ");

	// create thread
	TestThread10 t;

	// check the initial stack size and usage
	CU_ASSERT(256 == t.getTotalStackSize());
	CU_ASSERT(0 == t.getUsedStackSize());

	// run the thread
	CU_ASSERT(true == t.run());
	// wait a while
	k_msleep(10);
	// check thread usage during run
	CU_ASSERT(256 == t.getTotalStackSize());
	CU_ASSERT(0 < t.getUsedStackSize());
	CU_ASSERT(256 > t.getUsedStackSize());

	// make the thread end
	runFlag = 0;
	// wait for the thread to finish
	CU_ASSERT(true == t.join(200));
}
