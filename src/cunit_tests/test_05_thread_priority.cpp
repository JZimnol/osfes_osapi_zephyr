#include "CUnit.h"
#include "osapi.h"

static volatile int runFlag;

class TestThread5 : public osapi::Thread {
    public:
	TestThread5() : Thread(3, 256, osapi::JOINABLE)
	{
		;
	}

    private:
	virtual void job()
	{
		while (runFlag) {
			sleep(100);
		}
	}
};

void TEST_ThreadPriority(void)
{
	printf("Running test 05: thread priority set and get     ");

	// set initial flag values
	runFlag = 1;
	// create thread and run it
	TestThread5 t;
	CU_ASSERT(true == t.run());
	// check if initial priority was set
	CU_ASSERT(3 == t.getPriority());
	// test thread priority setting from outside the thread
	CU_ASSERT(true == t.setPriority(4));
	CU_ASSERT(4 == t.getPriority());
	// terminate thread
	runFlag = 0;
	t.join(1000);
}
