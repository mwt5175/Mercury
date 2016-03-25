/*****************************************************

	app.h
		Application

*****************************************************/

#include <iostream>
#include "appwin.h"

using namespace std;

namespace hgk {

bool HGCALL HgApplicationWin32::registerWindow (string winclass) {
	WNDCLASSEX wClass;
	wClass.hInstance     = GetModuleHandle(0);
	wClass.lpszClassName = winclass.c_str();
	wClass.lpfnWndProc   = DefWindowProc; //windowProc;
	wClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wClass.cbSize        = sizeof(WNDCLASSEX);
	wClass.hIcon         = NULL;
	wClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wClass.lpszMenuName  = NULL;
	wClass.hbrBackground = (HBRUSH)GetStockObject (BLACK_BRUSH);
	wClass.cbClsExtra    = 0;
	wClass.cbWndExtra    = 0;
	if (!RegisterClassEx(&wClass)) {
		return false;
	}
	return true;
}

/**
*	Swaps backbuffer to frontbuffer
*	\ret TRUE if success, FAIL if not
*/
bool HGCALL HgApplicationWin32::swapBuffers () {

	HDC hdc;

	if (!hwnd)
		return false;

	hdc = GetDC(hwnd);
	if (SwapBuffers(hdc))
		return true;
	return false;
}

/**
*	Initialize application window
*	\param title Window title
*	\param x Initial x
*	\param y Initial y
*	\param w Initial width
*	\param h Initial height
*	\param visable Visable flag
*	\param TRUE if success, FALSE if not
*/
bool HgApplicationWin32::createWindow (string classname, string caption, size_t x, size_t y, size_t w, size_t h) {

	hwnd = CreateWindowEx(NULL,classname.c_str(),caption.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		x, y,w, h,NULL,NULL,GetModuleHandle(0),NULL);

	if (!hwnd)
		return false;
	return true;
}

/**
*	Initialize application window
*	\param title Window title
*	\param x Initial x
*	\param y Initial y
*	\param w Initial width
*	\param h Initial height
*	\param TRUE if success, FALSE if not
*/
bool HgApplicationWin32::initialize (string title, size_t x, size_t y, size_t w, size_t h, bool visable) {

	if (!registerWindow ("Classname"))
		return false;

	if (!createWindow ("Classname",title,x,y,w,h))
		return false;

	setPixelFormat (32,24,8,0);

	return show (visable);
}

/**
*	Show or hide window
*	\param visable Visable flag
*	\ret TRUE if success, FALSE if not
*/
bool HGCALL HgApplicationWin32::show (bool visable) {

	BOOL result;

	if (!hwnd)
		return false;

	if (visable)
		result = ShowWindow(hwnd,SW_SHOW);
	else
		result = ShowWindow(hwnd,SW_HIDE);

	if(result)
		return true;
	return false;
}

/**
*	Sets pixel format
*	Defaults to 32 bit color format with 24 bit depth buffer and 8 bit stencil
*	\param bpp Bits per pixel
*	\param depth Depth buffer size
*	\param stencil Stencil buffer size
*	\param aux Aux buffer size
*	\ret TRUE if success, FAIL if not
*/
bool HgApplicationWin32::setPixelFormat (size_t bpp, size_t depth, size_t stencil, size_t aux) {

	HDC hdc;
	int result;

	if (!hwnd)
		return false;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, // framebuffer
		bpp,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		depth,         // buffer sizes
		stencil,
		aux,
		PFD_MAIN_PLANE,0,0, 0, 0
	};

	hdc    = GetDC (hwnd);
	result = ChoosePixelFormat(hdc, &pfd); 
	if (SetPixelFormat(hdc,result, &pfd))
		return true;
	return false;
}

/**
*	Set display settings
*	\param w Width
*	\param h Height
*	\param bpp Bits Per Pixel
*	\param fullscreen Enables fullscreen
*	\ret TRUE if success, FALSE if not
*/
bool HGCALL HgApplicationWin32::setDisplayMode (size_t w, size_t h, size_t bpp, bool fullscreen) {

	DEVMODE dmScreenSettings;
	DWORD   flags;
	DWORD   result;

	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
	dmScreenSettings.dmSize=sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth	= w;
	dmScreenSettings.dmPelsHeight	= h;
	dmScreenSettings.dmBitsPerPel	= bpp;
	dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

	if (fullscreen)
		flags = CDS_FULLSCREEN;
	else
		flags = 0;

	if (ChangeDisplaySettings(&dmScreenSettings,flags)!=DISP_CHANGE_SUCCESSFUL)
		return false;

	if (fullscreen)
		result = SetWindowLong(hwnd, GWL_STYLE,WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	else
		result = SetWindowLong(hwnd, GWL_STYLE,WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

	if (!result)
		return false;
	return true;
}

/**
*	Returns window device context handle
*	\ret Handle to device context
*/
HDC HGCALL HgApplicationWin32::getDeviceContext () {
	return GetDC (hwnd);
}

/**
*	Returns instance handle
*	\ret Handle to this applications instance
*/
HINSTANCE HGCALL HgApplicationWin32::getInstance () {
	return GetModuleHandle (0);
}

/**
*	Dispatch and translate win32 messages for this window
*	This is a blocking function.
*	\ret TRUE if message is processed, FALSE if processed message is WM_QUIT
*/
bool HGCALL HgApplicationWin32::dispatchMessages (bool wait) {
	MSG msg;
	BOOL result;

	if (wait)	result = GetMessage(&msg,0,0,0);
	else		result = PeekMessage(&msg,0,0,0,PM_REMOVE);

	if (result) {
		if (msg.message==WM_QUIT)
			return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

} //namespace hgk
