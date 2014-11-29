
#include "gfx3d_config.h"

////////////////////////////////////////////////////////////////////////////////
// OSAL definition

namespace avej_lite { namespace gfx3d
{
#if (TARGET_DEVICE == TARGET_TIZEN)
#else
	namespace osal
	{
		EGLNativeWindowType  GetNativeWindow(int screen, unsigned int width, unsigned int height, unsigned int depth);
		EGLNativeDisplayType GetNativeDC(void);
		void                 ReleaseNativeWindow(EGLNativeWindowType h_wnd);
		void                 ReleaseNativeDC(EGLNativeDisplayType h_dc);
		void                 ProcessMessage(unsigned long delay_time);
		bool                 PrintEGLError(const char* sz_message, int error);
		void                 ShowMessage(const char* sz_message, const char* sz_title = 0);
	}
#endif
}}

////////////////////////////////////////////////////////////////////////////////
// OSAL implementation

#if (TARGET_DEVICE == TARGET_WIN32)

#include <windows.h>
#include <stdio.h>

#define	WINDOW_CLASS "SMgalClass"

namespace
{
	HWND  g_h_wnd = 0;
	HDC   g_h_dc  = 0;
}

HWND g_GetWindowHandle(void)
{
	return g_h_wnd;
}

static LRESULT CALLBACK WndProc(HWND g_h_wnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	switch (message)
	{
#if 1
		case WM_KEYDOWN:
		{
			if (w_param == 27)
			{
				PostQuitMessage(0);
				return 1;
			}
			break;
		}
#endif
		case WM_SYSCOMMAND:
		{
			switch (w_param)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;
		}

		case WM_CLOSE:
			PostQuitMessage(0);
			return 1;

		default:
			break;
	}

	return DefWindowProc(g_h_wnd, message, w_param, l_param);
}

EGLNativeWindowType avej_lite::gfx3d::osal::GetNativeWindow(int screen, unsigned int width, unsigned int height, unsigned int depth)
{
	if (USE_ROTATION)
	{
		width ^= height;
		height ^= width;
		width ^= height;
	}

	HINSTANCE h_instance = GetModuleHandle(NULL);

	// windows class
	{
		WNDCLASS window_class;

		window_class.style         = CS_HREDRAW | CS_VREDRAW;
		window_class.lpfnWndProc   = WndProc;
		window_class.cbClsExtra    = 0;
		window_class.cbWndExtra    = 0;
		window_class.hInstance     = h_instance;
		window_class.hIcon         = 0;
		window_class.hCursor       = 0;
		window_class.lpszMenuName  = 0;
		window_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		window_class.lpszClassName = WINDOW_CLASS;

		ATOM registerClass         = RegisterClass(&window_class);

		if (!registerClass)
		{
			MessageBox(0, "Failed to register the window class", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
	}

	// eglWindow
	{
		::RECT	window_rect;
		SetRect(&window_rect, 0, 0, width, height);

		AdjustWindowRectEx(&window_rect, WS_CAPTION | WS_SYSMENU, false, 0);
		g_h_wnd = CreateWindow(WINDOW_CLASS, "sm+gal", WS_VISIBLE | WS_SYSMENU,
		                       100, 100, window_rect.right-window_rect.left, window_rect.bottom-window_rect.top, NULL, NULL, (HINSTANCE)h_instance, NULL);
//		                       950, 740, window_rect.right-window_rect.left, window_rect.bottom-window_rect.top, NULL, NULL, (HINSTANCE)h_instance, NULL);
	}

	return g_h_wnd;
}

EGLNativeDisplayType avej_lite::gfx3d::osal::GetNativeDC(void)
{
	g_h_dc = GetDC(g_h_wnd);
	return g_h_dc;
}

void avej_lite::gfx3d::osal::ReleaseNativeWindow(EGLNativeWindowType h_wnd)
{
	if (h_wnd)
		DestroyWindow((HWND)h_wnd);

	if (h_wnd == g_h_wnd)
		g_h_wnd = 0;
}

void avej_lite::gfx3d::osal::ReleaseNativeDC(EGLNativeDisplayType h_dc)
{
	if (h_dc)
		ReleaseDC(g_h_wnd, (HDC)h_dc);

	if (h_dc == g_h_dc)
		g_h_dc = 0;
}

void avej_lite::gfx3d::osal::ProcessMessage(unsigned long delay_time)
{
	MSG msg;

	while (::PeekMessage(&msg, g_h_wnd, NULL, NULL, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return;

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	::Sleep(delay_time);
}

bool avej_lite::gfx3d::osal::PrintEGLError(const char* sz_message, int error)
{
	char s[256];

	sprintf(s, "%s failed (%d).\n", sz_message, error);

	MessageBox(g_h_wnd, s, "Error", MB_OK | MB_ICONEXCLAMATION);

	return false;
}

void avej_lite::gfx3d::osal::ShowMessage(const char* sz_message, const char* sz_title)
{
	const char* sz_caption = (sz_title != NULL) ? sz_title : "Error";

	MessageBox(g_h_wnd, sz_message, sz_caption, MB_OK | MB_ICONEXCLAMATION);
}

#elif (TARGET_DEVICE == TARGET_GP2XWIZ)

// delete ?

#elif (TARGET_DEVICE == TARGET_BADA)

#include <stdlib.h>
#include <FBase.h>

extern void* g_hNativeWindow;


EGLNativeWindowType avej_lite::gfx3d::osal::GetNativeWindow(int screen, unsigned int width, unsigned int height, unsigned int depth)
{
	return (EGLNativeWindowType)g_hNativeWindow;
}

EGLNativeDisplayType avej_lite::gfx3d::osal::GetNativeDC(void)
{
	return (EGLNativeDisplayType)EGL_DEFAULT_DISPLAY;
}

void avej_lite::gfx3d::osal::ReleaseNativeWindow(EGLNativeWindowType h_wnd)
{
}

void avej_lite::gfx3d::osal::ReleaseNativeDC(EGLNativeDisplayType h_dc)
{
}

void avej_lite::gfx3d::osal::ProcessMessage(unsigned long delay_time)
{
}

bool avej_lite::gfx3d::osal::PrintEGLError(const char* sz_message, int error)
{
	AppLog("[SMGAL] EGL error(%d): %s", error, sz_message);
	return true;
}

void avej_lite::gfx3d::osal::ShowMessage(const char* sz_message, const char* sz_title)
{
	AppLog("[SMGAL] [%s]: %s", sz_title, sz_message);
}

#endif
