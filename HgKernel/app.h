/*****************************************************

	app.h
		Application

*****************************************************/

#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

/* include for operating system specific HgApplication framework */

#ifdef _WIN32
#include "appwin.h"
#else
#error _WIN32 not defined. Compilier not supported
#endif

namespace hgk {

/**
*	HgApplication is an operating system independent type
*	representing the application window and event loop.
*/

#ifdef _WIN32
typedef HgApplicationWin32 HgApplication;
#endif

}

#endif
