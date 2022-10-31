/**
* @file smutex.h
* @brief Sphere simple mutex implementation.
*/

#ifndef _INC_SMUTEX_H
#define _INC_SMUTEX_H

#include "../common.h"
#ifndef _WIN32
	#include <pthread.h>
#endif

#ifdef _BSD
	#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
#endif


/**
* Simple mutex class calling OS APIs directly.
*/
class SimpleMutex
{
public:
	/** @name Constructors, Destructor, Asign operator:
	*/
	///@{
	SimpleMutex();
	~SimpleMutex();
private:
	SimpleMutex(const SimpleMutex& copy);
	SimpleMutex& operator=(const SimpleMutex& other);
	///@}
public:
	/** @name Interaction:
	*/
	///@{
	/**
	* @brief Locks the mutex.
	*
	* Waits for ownership of the specified critical section object. The function
	* returns when the calling thread is granted ownership.
	*/
	inline void lock()
	{
#ifdef _WIN32
		EnterCriticalSection(&m_criticalSection);
#else
		pthread_mutex_lock(&m_criticalSection);
#endif
	}
	/**
	* @brief Tries to lock the mutex.
	*
	* Attempts to enter a critical section without blocking. If the call is
	* successful, the calling thread takes ownership of the critical section.
    * @return True If the critical section is successfully entered or the current
    * thread already owns the critical section, false otherwise.
	*/
	inline bool tryLock()
	{
#ifdef _WIN32
		return TryEnterCriticalSection(&m_criticalSection) == TRUE;
#else
		return pthread_mutex_trylock(&m_criticalSection) == 0;
#endif
	}
	/**
	* @brief Unlocks the mutex.
	*
	* Releases ownership of the specified critical section object.
	*/
	inline void unlock()
	{
#ifdef _WIN32
		LeaveCriticalSection(&m_criticalSection);
#else
		pthread_mutex_unlock(&m_criticalSection);
#endif
	}
	///@}

private:
#ifdef _WIN32
	CRITICAL_SECTION m_criticalSection;   // Windows API specific mutex.
#else
	pthread_mutex_t m_criticalSection;   // Unix API specific mutex.
	pthread_mutexattr_t m_criticalSectionAttr;   // Unix API mutex attr.
#endif

};

/**
* @brief A simple automatic thread lock.
*
* Locks on creation and unlocks on destruction.
*/
class SimpleThreadLock
{
public:
	/** @name Constructors, Destructor, Asign operator:
	*/
	///@{
	inline explicit SimpleThreadLock(SimpleMutex &mutex) : m_mutex(mutex), m_locked(true) {
		mutex.lock();
	}
	inline ~SimpleThreadLock() {
		m_mutex.unlock();
	}
private:
	SimpleThreadLock(const SimpleThreadLock& copy);
	SimpleThreadLock& operator=(const SimpleThreadLock& other);
	///@}
public:
	/** @name Operators:
	*/
	///@{
	/**
	* @brief Is the thread locked?
	*
	* In practice, this is always true.
	*/
	inline operator bool() const {
		return m_locked;
	}
	///@}

private:
	SimpleMutex &m_mutex;   // Mutex to control the lock.
	bool m_locked;    // Always true.
};

/**
*
*/
class ManualThreadLock
{
public:
	/** @name Constructors, Destructor, Asign operator:
	*/
	///@{
	inline ManualThreadLock() : m_mutex(nullptr), m_locked(false) {
	}
	inline explicit ManualThreadLock(SimpleMutex * mutex) : m_locked(false) {
		setMutex(mutex);
	}
	inline ~ManualThreadLock() {
		if (m_mutex != nullptr)
			doUnlock();
	}
private:
	ManualThreadLock(const ManualThreadLock& copy);
	ManualThreadLock& operator=(const ManualThreadLock& other);
	///@}
public:
	/** @name Modifiers:
	*/
	///@{
	/**
	* @brief Sets the mutex to modify.
	* @param mutex
	*/
	inline void setMutex(SimpleMutex * mutex) {
		m_mutex = mutex;
	}
	///@}
	/** @name Operators:
	*/
	///@{
	inline operator bool() const {
		return m_locked;
	}
	///@}
	/** @name Interaction:
	*/
	///@{
	/**
	* @brief Locks the mutex.
	*
	* Waits for ownership of the specified critical section object. The function
	* returns when the calling thread is granted ownership.
	*/
	void doLock();
	/**
	* @brief Tries to lock the mutex.
	*
	* Attempts to enter a critical section without blocking. If the call is
	* successful, the calling thread takes ownership of the critical section.
    * @return True If the critical section is successfully entered or the current
    * thread already owns the critical section, false otherwise.
	*/
	bool doTryLock();
	/**
	* @brief Unlocks the mutex.
	*
	* Releases ownership of the specified critical section object.
	*/
	void doUnlock();
	///@}

private:
	SimpleMutex * m_mutex;   // Mutex to control the lock.
	bool m_locked;    // Is the thread locked?
};


#endif // _INC_SMUTEX_H
