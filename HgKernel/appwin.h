/*****************************************************

	appwin.h
		WIN32 Application

*****************************************************/

#ifndef APPWIN_H_INCLUDED
#define APPWIN_H_INCLUDED

#ifndef _WIN32
#error _WIN32 not defined. Must have WIN32 compilier to build HgKernel/appwin
#endif

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRALEAN
#include <windows.h>

#include <string>
#include <hgstd/base.h>
#include <hgthread/thread.h>

namespace hgk {

/**
*	WIN32 Application
*	Provides basic WIN32 services for managing application windows & events
*/
class HgApplicationWin32 {

	HWND                hwnd;

	bool HGCALL  registerWindow (std::string winclass);

	bool HGCALL  createWindow (std::string classname, std::string caption, size_t x, size_t y, size_t w, size_t h);

public:

	HgApplicationWin32 () : hwnd(0) {}
	virtual ~HgApplicationWin32 () {}

	bool           HGCALL   initialize (std::string title, size_t x, size_t y, size_t w, size_t h, bool visable=false);

	bool           HGCALL   show (bool visable=true);

	bool           HGCALL   setPixelFormat (size_t bpp=32, size_t depth=24, size_t stencil=8, size_t aux=0);

	bool           HGCALL   swapBuffers ();

	bool           HGCALL   setDisplayMode (size_t w, size_t h, size_t bpp, bool fullscreen=false);

	bool           HGCALL   dispatchMessages (bool wait=true);

	HDC            HGCALL   getDeviceContext ();

	HINSTANCE      HGCALL   getInstance ();

	inline HWND    HGCALL   getWindowHandle () {return hwnd;}
};

}

#endif
