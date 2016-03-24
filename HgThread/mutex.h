/*****************************************************

	mutex.h
		Mutex

*****************************************************/

#ifndef MUTEX_H_INCLUDED
#define MUTEX_H_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <string>

namespace hg {

namespace thread {

/**
* Mutex object
*/
class HgMutex {

	// OS-dependent mutex object
	HANDLE hMutex;

	// name
	std::string name;

public:

	/**
	* Constructor. Creates a mutex
	* \param name Optional mutex name
	*/
	HgMutex (std::string name="");

	/**
	* Destructor. Releases the mutex
	*/
	virtual ~HgMutex ();

	/**
	* Locks mutex
	* \param timeout Timeout, in milliseconds, to wait for object lock
	* \ret true if successful lock, false if not
	*/
	virtual bool lock (int timeout=0);

	/**
	* Unlocks mutex
	* \ret true if successful, false if error
	*/
	virtual bool unlock ();

	/**
	* Returns mutex name
	* \ret mutex name, empty string if none
	*/
	virtual inline std::string getName() {
		return name;
	}

	/**
	* Sets mutex name
	* \param n new name for mutex
	*/
	virtual inline void setName (std::string n) {
		lock ();
		name=n;
		unlock();
	}

	/**
	* Comparision
	* \param m mutex object to compare with
	* \ret true if same mutex, false if not
	*/
	inline bool operator == (HgMutex& m) const {
		return (hMutex==m.hMutex);
	}
};

}

}

#endif
