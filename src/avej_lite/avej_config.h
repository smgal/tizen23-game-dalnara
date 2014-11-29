
#ifndef __AVEJ_CONFIG_H__
#define __AVEJ_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// type of TARGET_DEVICE

#define TARGET_WIN32    1
#define TARGET_GP2XWIZ  2
#define TARGET_BADA     3
#define TARGET_TIZEN    4

#if !defined(TARGET_DEVICE)
#	if defined(_MSC_VER)
#		define TARGET_DEVICE  TARGET_WIN32
#	elif defined(ARM9)
#		define TARGET_DEVICE  TARGET_GP2XWIZ
#	else
#		error  "add -DTARGET_DEVICE=TARGET_??? in your Makefile."
#	endif
#endif

////////////////////////////////////////////////////////////////////////////////
// type of DEVICE_RES

#define DEVICE_RES_800x480    1
#define DEVICE_RES_480x320    2
#define DEVICE_RES_320x240    3

#if   (DEVICE_RES == DEVICE_RES_800x480)
#	define SCREEN_WIDTH    800
#	define SCREEN_HEIGHT   480
#elif (DEVICE_RES == DEVICE_RES_480x320)
#	define SCREEN_WIDTH    480
#	define SCREEN_HEIGHT   320
#elif (DEVICE_RES == DEVICE_RES_320x240)
#	define SCREEN_WIDTH    320
#	define SCREEN_HEIGHT   240
#else
#	error  "add -DDEVICE_RES=DEVICE_RES_???x??? in your Makefile."
#endif

////////////////////////////////////////////////////////////////////////////////
// screen color depth

#if !defined(SCREEN_DEPTH)
#	define SCREEN_DEPTH     16
#endif

////////////////////////////////////////////////////////////////////////////////
//

// number of layer on screen
#define MAX_LAYER_IN_SURFACE 10

// whether memory manager is used
#define ENABLE_MEMORY_MANAGER

////////////////////////////////////////////////////////////////////////////////
// automatic modification by the configuration

#if defined(_MSC_VER)
#	define _CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
//#	define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#	undef  ENABLE_MEMORY_MANAGER
#	define ENABLE_MEMORY_MANAGER _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#endif // #ifndef __AVEJ_CONFIG_H__
