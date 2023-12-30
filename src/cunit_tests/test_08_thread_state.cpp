#include "CUnit.h"
#include "osapi.h"

static volatile osapi::ThreadState runState = osapi::UNKNOWN;
static volatile int runFlag = 1;

class TestThread08 : public osapi::Thread {
    public:
	TestThread08() : Thread(2, 256, osapi::JOINABLE)
	{
		;
	}

    private:
	virtual void job()
	{
		// check the state while running
		runState = getState();
		while (runFlag) {
			;
		}
		sleep(100);
	}
};

void TEST_ThreadStateChecking(void)
{
	printf("Running test 08: thread state checking           ");

	// create thread
	TestThread08 t;

	// at this point thread should be in INACTIVE state
	CU_ASSERT(osapi::INACTIVE == t.getState());
	// run the thread
	CU_ASSERT(true == t.run());
	// wait a while
	k_msleep(5);
	// at this point runState should be set to RUNNING state,
	// but Zephyr does not signal such state
	CU_ASSERT(osapi::READY == runState);
	// at this point thread should be in READY state (inside the while loop)
	CU_ASSERT(osapi::READY == t.getState());
	// suspend the thread
	CU_ASSERT(true == t.suspend());
	// at this point thread should be in SUSPENDED state
	CU_ASSERT(osapi::SUSPENDED == t.getState());
	// resume the thread
	CU_ASSERT(true == t.resume());
	// wait for the thread to be resumed
	k_msleep(5);
	// at this point thread should be in READY state (inside the while loop)
	CU_ASSERT(osapi::READY == t.getState());

	// make the thread end
	runFlag = 0;
	// wait a while so that the thread can enter sleep
	k_msleep(5);
	// at this point thread should be in BLOCKED state (during sleep)
	CU_ASSERT(osapi::BLOCKED == t.getState());
	// wait for the thread to finish
	CU_ASSERT(true == t.join(200));
	// at this point thread should be again in INACTIVE state
	CU_ASSERT(osapi::INACTIVE == t.getState());
}
