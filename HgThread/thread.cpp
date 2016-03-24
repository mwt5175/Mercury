/*****************************************************

	HgThread.cpp
		Thread Support

*****************************************************/

#include <iostream>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "thread.h"

namespace hg {

namespace thread {

LRESULT HgThreadManager::ProcessThread(LPVOID parm) {

	//! call objects run method to start HgThread
	HgThread* pThread = reinterpret_cast<HgThread*> (parm);
	return pThread->run (0);
}

HgThreadManager::~HgThreadManager () {

	terminateAll ();
	pkThreads.clear ();
}

void HgThreadManager::addThread (HgThread* pThread) {

	if (pThread)
		pkThreads.push_back (HgThreadSharedPtr(pThread));
}

void HgThreadManager::removeThread (HgThread* pThread) {

	if (pThread) {

		std::vector<HgThreadSharedPtr>::iterator it = pkThreads.begin();
		for ( ; it != pkThreads.end (); it++)
			if ((*it).get() == pThread) {

				pkThreads.erase (it);
				return;
			}
	}
}

void HgThreadManager::terminateThread (HgThread* pThread) {

	if (pThread) {

		//! does manager own HgThread?
		for (unsigned int i=0; i < pkThreads.size(); i++)
			if (pkThreads[i]->getId() == pThread->getId() ) {

				//terminate HgThread
				HANDLE handle = pThread->getData ();
				TerminateThread ( handle, 0);
			}
	}
}

void HgThreadManager::terminateAll () {

	for (unsigned int i=0; i < pkThreads.size(); i++) {
		HANDLE handle = pkThreads[i]->getData ();
		TerminateThread ( handle, 0);
	}
}

void HgThreadManager::createThread (HgThread* pkThread) {
	if (pkThread) {

		unsigned int tid=0;
		HANDLE handle = CreateThread (0,0,(LPTHREAD_START_ROUTINE) HgThreadManager::ProcessThread,
			pkThread,0, (LPDWORD) tid);

		pkThread->data = (void*) handle;
		pkThread->id = tid;
	}
}

void HgThreadManager::suspend (HgThread* pkThread) {
	if (pkThread) {
		HANDLE handle = pkThread->getData ();
		SuspendThread ( handle );
	}

}

void HgThreadManager::resume (HgThread* pkThread) {

	if (pkThread) {
		HANDLE handle = pkThread->getData ();
		ResumeThread ( handle );
	}
}

HgThreadSharedPtr HgThreadManager::get (unsigned int id) {

	for (unsigned int i=0; i<pkThreads.size(); i++)
		if (pkThreads[i]->getId() == id)
			return pkThreads[i];

	// returns null shared pointer
	return HgThreadSharedPtr();
}

HgThreadSharedPtr HgThreadManager::get (std::string name) {

	for (unsigned int i=0; i<pkThreads.size(); i++)
		if (pkThreads[i]->getName() == name)
			return pkThreads[i];

	// returns null shared pointer
	return HgThreadSharedPtr();
}

}
}
