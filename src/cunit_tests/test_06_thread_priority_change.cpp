#include "CUnit.h"
#include "osapi.h"

static volatile int runFlag = 1;

static volatile int osapiInitialPriorityA = -999;
static volatile int osapiInitialPriorityB = -999;
static volatile int osapiFinalPriorityA = -999;
static volatile int osapiFinalPriorityB = -999;
static volatile int priorityDifferenceA = 0;
static volatile int priorityDifferenceB = 0;

class TestThread06A : public osapi::Thread {
    public:
	TestThread06A() : Thread(3, 256, osapi::JOINABLE)
	{
		run();
	}

    private:
	virtual void job()
	{
		// get initial priority using osapi
		osapiInitialPriorityA = getPriority();
		// get initial priority using Zephyr API
		unsigned int p1 = k_thread_priority_get(k_current_get());
		// change priority using osapi
		setPriority(5);
		// get new priority using Zephyr API
		unsigned int p2 = k_thread_priority_get(k_current_get());
		// calculate difference
		priorityDifferenceA = p2 - p1;
		while (runFlag) {
			sleep(5);
		}
	}
};

class TestThread06B : public osapi::Thread {
    public:
	TestThread06B() : Thread(4, 256, osapi::JOINABLE)
	{
		run();
	}

    private:
	virtual void job()
	{
		// get initial priority using osapi
		osapiInitialPriorityB = getPriority();
		// get initial priority using Zephyr API
		unsigned int p1 = k_thread_priority_get(k_current_get());
		// change priority using osapi
		setPriority(7);
		// get new priority using Zephyr API
		unsigned int p2 = k_thread_priority_get(k_current_get());
		priorityDifferenceB = p2 - p1;
		// calculate difference
		while (runFlag) {
			sleep(5);
		}
	}
};

void TEST_ThreadPriorityChange(void)
{
	printf("Running test 06: thread priority change          ");

	// create 2 threads with initial priorities 3 and 4
	TestThread06A tA;
	TestThread06B tB;
	k_msleep(100);
	CU_ASSERT(3 == osapiInitialPriorityA);
	CU_ASSERT(4 == osapiInitialPriorityB);
	CU_ASSERT(5 == tA.getPriority());
	CU_ASSERT(7 == tB.getPriority());
	CU_ASSERT(2 == priorityDifferenceA);
	CU_ASSERT(3 == priorityDifferenceB);
	runFlag = 0;
	CU_ASSERT(true == tA.join(100));
	CU_ASSERT(true == tB.join(100));
}
