/*****************************************************

	thread.h
		Thread Support

*****************************************************/

#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <vector>
#include <string>

namespace hg {

namespace thread {

/**
* Thread object
*/
class HgThread {

	unsigned int id;
	std::string  name;

	// thread specific paramaters
	void*        parms;

	// OS specific data
	void*        data;

public:

	/**
	* Returns thread ID
	* \ret thread ID
	*/
	inline unsigned int getId () const {
		return id;
	}

	/**
	* Returns thread name
	* \return thread name
	*/
	inline const std::string&  getName () const {
		return name;
	}

	/**
	* Sets thread name
	* \param n New thread name
	*/
	inline void setName (std::string& n) {
		name=n;
	}

	/**
	* Returns thread paramaters
	* \return Raw pointer to paramater data
	*/
	virtual inline void* getParms () {
		return parms;
	}

	/**
	* Set paramater data
	* \param p Raw pointer to paramater data
	*/
	virtual inline void setParms (void* p) {
		parms=p;
	}

	/**
	* Returns thread paramaters
	* \return Raw pointer to paramater data
	*/
	virtual inline void* getData () {
		return parms;
	}

	/**
	* Thread start routine.
	* \return Thread return code
	*/
	virtual int run (void*) = 0;

	friend class HgThreadManager;
};

typedef std::tr1::shared_ptr<HgThread>  HgThreadSharedPtr;
typedef std::vector <HgThreadSharedPtr> HgThreadList;

/**
* Thread manager
*/
class HgThreadManager {

	HgThreadList pkThreads;

#ifdef _WIN32
	static LRESULT ProcessThread(LPVOID parm);
#endif

public:

	virtual ~HgThreadManager ();

	/**
	* Adds a thread
	* \param p Pointer to created thread object. The thread is not started.
	*/
	virtual void    addThread (HgThread* p);

	/**
	* Removes a thread
	* \param p Pointer to created thread object to remove. The thread is not stopped.
	*/
	virtual void    removeThread (HgThread* p);

	/**
	* Terminates a thread
	* \param p Thread to stop. Call removeThread to unmanage it
	*/
	virtual void    terminateThread (HgThread* p);

	/**
	* Terminates all threads
	* \param p Stops all threads. Call removeThread to unmanage threads
	*/
	virtual void    terminateAll ();

	/**
	* Create thread object
	* \param p Thread object to create. This doesnt start the thread
	*/
	virtual void    createThread (HgThread* p);

	/**
	* Suspend execution
	* \param p Thread
	*/
	virtual void    suspend (HgThread* p);

	/**
	* Resume execution
	* \param p Thread
	*/
	virtual void    resume (HgThread* p);

	/**
	* Returns number of threads
	* \return thread count
	*/
	inline unsigned int getCount () {
		return pkThreads.size();
	}

	/**
	* Returns thread by id
	* \param id Thread id
	* \ret pointer to thread object
	*/
	virtual HgThreadSharedPtr get (unsigned int id);

	/**
	* Returns thread by name
	* \param name thread name
	* \ret pointer to thread object
	*/
	virtual HgThreadSharedPtr get (std::string name);
};

}

}

#endif
