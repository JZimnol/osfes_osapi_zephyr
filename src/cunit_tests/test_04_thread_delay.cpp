#include "CUnit.h"
#include "osapi.h"

class TestThread4 : public osapi::Thread {
    public:
	TestThread4() : Thread(3, 256, osapi::JOINABLE)
	{
		;
	}

    private:
	virtual void job()
	{
		sleep(3000);
	}
};

void TEST_ThreadSleep(void)
{
	unsigned int start, stop;

	printf("Running test 04: thread sleep                    ");

	// mark start time
	start = osapi::getSystemTime();
	// create thread and run it
	TestThread4 t;
	CU_ASSERT(true == t.run());
	// wait for the thread to end
	t.join(5000);
	// mark stop time
	stop = osapi::getSystemTime();
	// check timings
	CU_ASSERT(stop - start > 2900);
	CU_ASSERT(stop - start < 3100);
}
