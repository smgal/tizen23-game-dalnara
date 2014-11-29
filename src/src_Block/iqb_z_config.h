
#ifndef __IQB_Z_CONFIG_H__
#define __IQB_Z_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// uses

#include "avej_lite.h"

namespace erio
{
	enum TState
	{
		STATE_TEAM_LOGO = 0,
		STATE_TITLE,
		STATE_OPTION,
		STATE_STAGE_SELECT,
		STATE_GAME_PLAY,
		STATE_ENDING,
		STATE_EXIT
	};

	extern void g_ChangeState(TState state, unsigned long param1 = 0);
}

////////////////////////////////////////////////////////////////////////////////
// multi-character

//#define USE_WCHAR

#if defined(USE_WCHAR)
#	include <wchar.h>
	typedef wchar_t tchar;
#	define TCHAR(s)   L##s

#else
	typedef char tchar;
#	define TCHAR(s)   s

#endif

#endif // #ifndef __IQB_Z_CONFIG_H__
