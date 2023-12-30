#ifndef OSAPI_RECURSIVE_MUTEX_ZEPHYR_H
#define OSAPI_RECURSIVE_MUTEX_ZEPHYR_H

#include "osapi_mutex_interface.h"

namespace osapi
{

class RecursiveMutex : public MutexInterface {
    private:
	struct k_mutex _mutex;

    public:
	RecursiveMutex()
	{
		k_mutex_init(&_mutex);
	}
	virtual bool lock(unsigned int timeout)
	{
		return !k_mutex_lock(&_mutex, K_MSEC(timeout));
	}

	virtual void unlock()
	{
		k_mutex_unlock(&_mutex);
	}
};

} // namespace osapi

#endif // OSAPI_RECURSIVE_MUTEX_ZEPHYR_H
