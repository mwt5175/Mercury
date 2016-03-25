/*****************************************************

	kernel.h
		Kernel

*****************************************************/

#ifndef KERNEL_H_INCLUDED
#define KERNEL_H_INCLUDED

#include <iostream>
#include <string>
#include <hgstd/base.h>
#include <hgthread/thread.h>

#ifdef _WIN32
#include <windows.h>
#define WIN32_LEAN_AND_MEAN
#endif

#include "app.h"

namespace hg {

extern thread::HgThreadManager _ThreadManager;

/**
*	Engine Kernel
*/
class HgKernel {

public:

	static bool initialize ();

	bool eventLoop ();

	friend class HgKernelCore;
};

}

/**
*	Application defined entry point
*	\param kernel Pointer to engine kernel object
*	\param argc Command line argument count
*	\param argv Command line arguments
*	\ret Status
*/
extern int HgApplicationEntry (hg::HgKernel* kernel, int argc, char** argv);

#endif
