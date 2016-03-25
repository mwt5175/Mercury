/*****************************************************

	kernel.cpp
		Kernel

*****************************************************/

#include "app.h"
#include "kernel.h"
#include <hgthread/thread.h>

#include <iostream>
using namespace std;

#pragma comment(lib, "hgthread.lib")
#pragma comment(lib, "hggraphics.lib")

namespace hg {

thread::HgThreadManager _ThreadManager;



bool HgKernel::initialize () {

	// start application thread
	HANDLE h = CreateThread(0,0,HgApplicationThreadEntry,this,0,0);

	// create default hidden window
	app.initialize("Mercury Engine",0,0,640,480,true);

	// raise our thread priority
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_ABOVE_NORMAL);

	return false;
}

}
