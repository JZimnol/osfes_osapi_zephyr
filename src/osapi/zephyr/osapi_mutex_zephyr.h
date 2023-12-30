#ifndef OSAPI_MUTEX_ZEPHYR_H
#define OSAPI_MUTEX_ZEPHYR_H

#include "osapi_mutex_interface.h"

namespace osapi
{

class Mutex : public MutexInterface {
    private:
	struct k_sem _sem;

    public:
	Mutex()
	{
		k_sem_init(&_sem, 1, 1);
	}
	virtual bool lock(unsigned int timeout)
	{
		return !k_sem_take(&_sem, K_MSEC(timeout));
	}

	virtual void unlock()
	{
		k_sem_give(&_sem);
	}
};

} // namespace osapi

#endif // OSAPI_MUTEX_ZEPHYR_H
