/*****************************************************

	HgMutex.cpp
		Mutex

*****************************************************/

#include "mutex.h"

namespace hg {

namespace thread {

/**
*	Create HgMutex
*/
HgMutex::HgMutex (std::string name) : hMutex(0) {

	hMutex = CreateMutex (0,FALSE,
		( name.length() > 1 ) ? ( name.c_str() ) : 0);

	this->name = name;
}

/**
*	Release HgMutex
*/
HgMutex::~HgMutex () {
	if (hMutex)
		CloseHandle (hMutex);
}

/**
*	Lock HgMutex
*/
bool HgMutex::lock (int timeout) {

	switch(WaitForSingleObject(hMutex, timeout)) {
		case WAIT_OBJECT_0: //thread got ownership of the HgMutex
			return true;
		case WAIT_ABANDONED: //thread got ownership of an abandoned HgMutex
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			return false;
	}
	return false;
}

/**
*	Unlock HgMutex
*/
bool HgMutex::unlock () {
	return ReleaseMutex (hMutex);
}

}
}
