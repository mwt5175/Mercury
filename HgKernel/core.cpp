/*****************************************************

	core.cpp
		Engine entry point and kernel core

*****************************************************/

#include "kernel.h"

namespace hg {

/**
*	Application thread entry point
*	\ret TRUE if success, FALSE otherwise
*/
static DWORD WINAPI HgApplicationThreadEntry (void* Param) {

	// calls real application defined entry point
	if (HgApplicationEntry  ((HgKernel*) Param, __argc, __argv))
		return 0;

	return 1;
}

class HgKernelCore : public HgKernel {

public:

	/**
	*	Kernel event loop
	*	\ret Status
	*/
	bool eventLoop () {

		// just loop forever dispatching application events
		while (true) {
			if (!app.dispatchMessages())
				return true;
		}
		return true;
	}

	/**
	*	Kernel entry point
	*	\ret TRUE if success, FALSE if not
	*/
	bool run () {

		// create default hidden window
		app.initialize("Mercury Engine",0,0,640,480,true);

		// start application thread
		HANDLE h = CreateThread(0,0,HgApplicationThreadEntry,this,0,0);

		// raise our thread priority
		SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_ABOVE_NORMAL);

		while(1) {

		}

		// enter event loop
		return true; //eventLoop ();
	}
};

static HgKernelCore core;

// namespace hg
}

/**
*	Engine entry point
*	\ret Status
*/
int main () {

	/* call kernel entry point */
	return hg::core.run ();
}
