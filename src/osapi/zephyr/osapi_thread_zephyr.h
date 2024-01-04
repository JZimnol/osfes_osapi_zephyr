#ifndef OSAPI_THREAD_ZEPHYR_H
#define OSAPI_THREAD_ZEPHYR_H

#include "osapi.h"

namespace osapi
{

/** Thread interface implementation for Zephyr. */
class Thread : public ThreadInterface {
    private:
	int _threadPriority;
	unsigned int _stackSize;
	Joinable _isJoinable;
	struct k_thread _threadStruct;
	k_tid_t _threadId;
	k_thread_stack_t *_threadStack;
	bool _isBlocked;

    public:
	/** Thread constructor.
	 *  @param[in] priority thread priority
	 *  @param[in] stackSize thread stack size in bytes
	 *  @param[in] isJoinable decides if the thread supports join operation or not
	 *  @param[in] name optional thread name
	 */
	Thread(int priority, unsigned int stackSize, Joinable isJoinable,
	       const char *name = "unnamed")
		: _threadPriority(priority), _stackSize(stackSize), _isJoinable(isJoinable),
		  _threadStruct(), _threadId(nullptr), _threadStack(nullptr), _isBlocked(false)
	{
		_threadStack = k_thread_stack_alloc(_stackSize, 0);
		if (!_threadStack) {
			return;
		}

		_threadId =
			k_thread_create(&_threadStruct, _threadStack, _stackSize, threadFunction,
					this, nullptr, nullptr, _threadPriority, 0, K_FOREVER);
		if (!_threadId) {
			return;
		}

		(void)k_thread_name_set(_threadId, name);
	}

	/** Virtual destructor required to properly destroy derived class objects. */
	virtual ~Thread()
	{
		if (_threadStack) {
			(void)k_thread_stack_free(_threadStack);
		}
	}

	/** Runs the thread.
	*  @retval true if the thread was started successfully,
	*  @retval false if the thread was not started successfully, or the thread was already running
	*/
	virtual bool run()
	{
		if (isRunning()) {
			return false;
		}

		if (!_threadId) {
			return false;
		}

		k_thread_start(_threadId);

		return true;
	}

	/** Checks if the thread is running.
	*  @retval true if the thread is running
	*  @retval false if the thread is not running
	*/
	virtual bool isRunning()
	{
		ThreadState state = getState();
		return (state != INACTIVE);
	}

	/** Waits for the thread to finish executing, with a given timeout.
	 *  @param timeout[in] number of milliseconds to wait for the thread to finish executing
	 *  @retval true if the thread was successfully joined in the given time
	 *  @retval false if the thread was not joined within the given time or the thread is not joinable at all
	 */
	virtual bool join(unsigned int timeout)
	{
		return !k_thread_join(&_threadStruct, K_MSEC(timeout));
	}

	/** Checks, if the thread is joinable.
	 *  @retval true if the thread is joinable
	 *  @retval false if the thread is not joinable
	 */
	virtual bool isJoinable()
	{
		return (_isJoinable == JOINABLE);
	}

	/** Suspends thread execution.
	 *  @retval true if the thread was suspended successfully
	 *  @retval false if the thread was not suspended for some reason
	 */
	virtual bool suspend()
	{
		ThreadState state = getState();
		if (state == SUSPENDED || state == INACTIVE) {
			return false;
		}
		k_thread_suspend(_threadId);
		return true;
	}

	/** Resumes thread execution.
	 *  @retval true if the thread was resumed successfully
	 *  @retval false if the thread was not resumed for some reason
	 */
	virtual bool resume()
	{
		ThreadState state = getState();
		if (state == INACTIVE) {
			return false;
		}
		k_thread_resume(_threadId);
		return true;
	}

	/** Sets thread priority
	 *  @param[in] priority new thread priority
	 *  @retval true if the priority for the thread was set successfully
	 *  @retval false if the priority for the thread was not set successfully for some reason
	 */
	virtual bool setPriority(int priority)
	{
		k_thread_priority_set(_threadId, priority);
		return true;
	}

	/** Gets the thread priority
	 *  @return current thread priority
	 */
	virtual int getPriority()
	{
		return k_thread_priority_get(_threadId);
	}

	/** Gets thread name
	 *  @return name of the thread
	 */
	virtual const char *getName()
	{
		return k_thread_name_get(_threadId);
	}

	/** Gets the current state of the thread
	 *  @return current state of the thread
	 */
	virtual ThreadState getState()
	{
		// This one is messy, but there was no other way to map osapi::ThreadState to
		// Zephyr's thread states.
		char buf[32];
		ThreadState ret = UNKNOWN;

		const char *state_str = k_thread_state_str(_threadId, buf, 32);

		if (k_current_get() == _threadId) {
			ret = RUNNING;
		} else if (_isBlocked) {
			ret = BLOCKED;
		} else if (strstr(state_str, "dead") != nullptr ||
			   strstr(state_str, "pending+suspended") != nullptr ||
			   strstr(state_str, "prestart") != nullptr) {
			ret = INACTIVE;
		} else if (strstr(state_str, "pending") != nullptr ||
			   strstr(state_str, "queued") != nullptr) {
			ret = READY;
		} else if (strstr(state_str, "suspended") != nullptr) {
			ret = SUSPENDED;
		}

		return ret;
	}

	/** Gets the total size of the stack assigned for this thread
	 *  @return total stack size in number of bytes
	 */
	virtual unsigned int getTotalStackSize()
	{
		return _stackSize;
	}

	/** Gets the size of the used stack for this thread
	 *  @return number of bytes used on the stack
	 */
	virtual unsigned int getUsedStackSize()
	{
		if (!isRunning()) {
			return 0;
		}

		size_t unused_space;
		int ret = k_thread_stack_space_get(&_threadStruct, &unused_space);

		return !ret ? _stackSize - unused_space : -1;
	}

	static void threadFunction(void *arg1, void *arg2, void *arg3)
	{
		Thread *osapiThreadObject = reinterpret_cast<Thread *>(arg1);
		if (osapiThreadObject) {
			osapiThreadObject->job();
		}
	}

    protected:
	/** Delays thread execution for a given time.
	 *  @param time[in] number of milliseconds to delay thread execution
	 */
	virtual void sleep(unsigned int time)
	{
		_isBlocked = true;
		k_msleep(time);
		_isBlocked = false;
	}
};

} // namespace osapi

#endif // OSAPI_THREAD_ZEPHYR_H
