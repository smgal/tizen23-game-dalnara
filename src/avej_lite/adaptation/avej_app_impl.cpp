
#include "../avej_config.h"

namespace avej_lite { namespace gfx3d
{
	bool InitOpenGL(unsigned int width, unsigned int height, unsigned int depth);
	void DoneOpenGL(void);
	void SwapBuffers(void);
	void ProcessMessage(unsigned long delay_time);
}}


#if (TARGET_DEVICE == TARGET_GP2XWIZ)
#	include <unistd.h>
#endif

void Terminate(void)
{
#if (TARGET_DEVICE == TARGET_GP2XWIZ)
	chdir("/usr/gp2x");
#if defined(_CAANOO_)
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", "--view-main", NULL);
#else
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#endif

#endif
}

//////////////////////////////

#include "../avej_app.h"
#include <stdlib.h>

class IAvejAppImpl: public avej_lite::IAvejApp
{
public:
	IAvejAppImpl(const avej_lite::AppCallback& callback, unsigned long param)
		: _fn_callback(callback)
	{
        //atexit(Terminate);
       
		if (_fn_callback.OnCreate)
			_fn_callback.OnCreate(param);
	}
	~IAvejAppImpl(void)
	{
		if (_fn_callback.OnDestory)
			_fn_callback.OnDestory();
	}

	void ProcessMessages(void)
	{
		::avej_lite::gfx3d::ProcessMessage(6);
//		IAvejApp::ProcessMessages();
	}

	bool Process(void)
	{
#if 1
	#if (TARGET_DEVICE == TARGET_WIN32)
		ProcessMessages();
	#endif
		return (_fn_callback.OnProcess) ? _fn_callback.OnProcess() : false;
#else
		_done = false;
		while (!_done)
		{
			ProcessMessages();

			if (_fn_callback.OnProcess)
				_done = !_fn_callback.OnProcess();
		}

#endif
	}

	static avej_lite::IAvejApp* p_app_impl;
	static int                  ref_count;
	static avej_lite::IAvejApp* GetInstance(const avej_lite::AppCallback& fn_callback);
	static avej_lite::IAvejApp* GetInstance(const avej_lite::AppCallback& fn_callback, unsigned long param1);
	static int                  Release(void);

protected:
	avej_lite::AppCallback _fn_callback;
	bool                   _done;

};

avej_lite::IAvejApp* IAvejAppImpl::p_app_impl = 0;
int                  IAvejAppImpl::ref_count  = 0;

avej_lite::IAvejApp* IAvejAppImpl::GetInstance(const avej_lite::AppCallback& fn_callback)
{
	return GetInstance(fn_callback, 0);
}

avej_lite::IAvejApp* IAvejAppImpl::GetInstance(const avej_lite::AppCallback& fn_callback, unsigned long param1)
{
	if (p_app_impl == 0)
	{
    	p_app_impl = new IAvejAppImpl(fn_callback, param1);
	}

	++ref_count;

	return p_app_impl;
}

int IAvejAppImpl::Release(void)
{
	if (--ref_count == 0)
	{
		delete (IAvejAppImpl*)p_app_impl;
		p_app_impl = NULL;
	}

	return ref_count;
}

//////////////////////////////////////////////////////////////////////////

avej_lite::IAvejApp* avej_lite::IAvejApp::GetInstance(const avej_lite::AppCallback& fn_callback)
{
	return IAvejAppImpl::GetInstance(fn_callback);
}

avej_lite::IAvejApp* avej_lite::IAvejApp::GetInstance(const avej_lite::AppCallback& fn_callback, unsigned long param1)
{
	return IAvejAppImpl::GetInstance(fn_callback, param1);
}

int avej_lite::IAvejApp::Release(void)
{
	return IAvejAppImpl::Release();
}

void avej_lite::IAvejApp::ProcessMessages(void)
{
	avej_lite::AppCallback dummy;

	IAvejAppImpl* pApp = static_cast<IAvejAppImpl*>(IAvejAppImpl::GetInstance(dummy));

	pApp->ProcessMessages();
	pApp->Release();
}

////////////////////////////////////////////////////////////////////////////////
// implementation

#if (TARGET_DEVICE == TARGET_WIN32)

#include <windows.h>

using namespace avej_lite;

namespace
{
	unsigned short s_input_key[INPUT_KEY_MAX];

	const unsigned short VIRTUAL_KEY_TABLE[INPUT_KEY_MAX] =
	{
		VK_UP,                 // INPUT_KEY_UP,
		VK_DOWN,               // INPUT_KEY_DOWN,
		VK_LEFT,               // INPUT_KEY_LEFT,
		VK_RIGHT,              // INPUT_KEY_RIGHT,
		(unsigned short)('A'), // INPUT_KEY_A,
		(unsigned short)('S'), // INPUT_KEY_B,
		(unsigned short)('D'), // INPUT_KEY_C,
		(unsigned short)('F'), // INPUT_KEY_D,
		VK_LSHIFT,             // INPUT_KEY_L1,
		VK_RSHIFT,             // INPUT_KEY_R1,
		VK_ESCAPE,             // INPUT_KEY_SYS1,
		VK_RETURN,             // INPUT_KEY_SYS2,
		(unsigned short)('X'), // INPUT_KEY_SYS1,
		(unsigned short)('Z'), // INPUT_KEY_SYS2,
	};

	const struct
	{
		unsigned short delegate_key;
		TInputKey      extended_key[2];
	}
	EXTENDED_KEY_TABLE[] =
	{
		{ (unsigned short)('Q'), { INPUT_KEY_UP,   INPUT_KEY_LEFT  } },
		{ (unsigned short)('E'), { INPUT_KEY_UP,   INPUT_KEY_RIGHT } },
		{ (unsigned short)('C'), { INPUT_KEY_DOWN, INPUT_KEY_RIGHT } },
		{ (unsigned short)('Z'), { INPUT_KEY_DOWN, INPUT_KEY_LEFT  } }
	};

	TMultiTouchInfo s_multi_touch_info;
}

CInputDevice::CInputDevice()
{
	memset(s_input_key, 0, sizeof(s_input_key));
}

CInputDevice::~CInputDevice()
{
}

extern HWND g_GetWindowHandle(void);

static void s_GetCursorPosLocal(int& x, int& y)
{
	POINT cursor_pos;
	GetCursorPos(&cursor_pos);
	
	RECT  window_rect;
	GetWindowRect(g_GetWindowHandle(), &window_rect);

	RECT  adjust_rect;
	SetRect(&adjust_rect, 0, 0, 10, 10);
	AdjustWindowRectEx(&adjust_rect, WS_CAPTION | WS_SYSMENU, false, 0);

	x = cursor_pos.x - window_rect.left + adjust_rect.left;
	y = cursor_pos.y - window_rect.top  + adjust_rect.top;
}

void CInputDevice::UpdateInputState()
{
	for (unsigned short virtual_key = 0; virtual_key < INPUT_KEY_MAX; ++virtual_key)
	{
		s_input_key[virtual_key] = GetAsyncKeyState(VIRTUAL_KEY_TABLE[virtual_key]);
	}

	// in case of [one key, multi input]
	for (int index = 0; index < sizeof(EXTENDED_KEY_TABLE) / sizeof(EXTENDED_KEY_TABLE[0]); ++index)
	{
		unsigned short key = GetAsyncKeyState(EXTENDED_KEY_TABLE[index].delegate_key);
		if (key)
		{
			s_input_key[EXTENDED_KEY_TABLE[index].extended_key[0]] = key;
			s_input_key[EXTENDED_KEY_TABLE[index].extended_key[1]] = key;
		}
	}

	s_multi_touch_info.x = -1;
	s_multi_touch_info.y = -1;

	if (HIBYTE(GetAsyncKeyState(VK_LBUTTON)) > 0)
	{
		s_GetCursorPosLocal(s_multi_touch_info.x, s_multi_touch_info.y);
	}
}

bool CInputDevice::IsKeyHeldDown(TInputKey key) const
{
	if (int(key) < INPUT_KEY_MAX)
	{
		return HIBYTE(s_input_key[key]) > 0;
	}
	return false;
}

bool CInputDevice::WasKeyPressed(TInputKey key) const
{
	if (int(key) < INPUT_KEY_MAX)
	{
		return LOBYTE(s_input_key[key]) > 0;
	}
	return false;
}

bool CInputDevice::GetMultiTouchInfo(TMultiTouchInfoArray& multi_touch_info_array) const
{
	if ((s_multi_touch_info.x < 0) || (s_multi_touch_info.y < 0))
		return false;

	multi_touch_info_array[0]   = s_multi_touch_info;
	multi_touch_info_array[1].x = -1;
	multi_touch_info_array[1].y = -1;

	return true;
}

#elif (TARGET_DEVICE == TARGET_GP2XWIZ)

// delete ?

#elif (TARGET_DEVICE == TARGET_BADA)

#include <FUiTouch.h>

using namespace avej_lite;

extern void* g_hNativeWindow;

Osp::Ui::Touch* g_touch = 0;

const int NUM_KEY = 7;

bool g_key_state[NUM_KEY];
bool g_key_pressed[NUM_KEY];

CInputDevice::CInputDevice()
{
	g_touch = new Osp::Ui::Touch;
	g_touch->SetMultipointEnabled(*((Osp::Ui::Control*)g_hNativeWindow), true);

	for (int i = 0; i < NUM_KEY; i++)
	{
		g_key_state[i] = false;
		g_key_pressed[i] = false;
	}
}

CInputDevice::~CInputDevice()
{
	delete g_touch;
}

struct TPos
{
	int x, y;
};

struct TRect
{
	int x1, y1, x2, y2;
};

bool s_InRect(int x, int y, int x1, int y1, int x2, int y2)
{
	return (x >= x1 && x < x2 && y >= y1 && y < y2);
}

bool s_InRect(int x, int y, const TRect& rect)
{
	return (x >= rect.x1 && x < rect.x2 && y >= rect.y1 && y < rect.y2);
}

bool s_InRange(int x, int y, const TPos& pos, int min, int max)
{
	int diff_x = x - pos.x;
	int diff_y = y - pos.y;

	int distance = diff_x * diff_x + diff_y * diff_y;
	return (distance >= min * min) && (distance >= max * max);
}


#if 0

const int WIDTH  = 800;
const int HEIGHT = 480;

const int BASE_X = 300;
const int BASE_SIZE = 50;

const int X0 = BASE_X + BASE_SIZE * 0;
const int X1 = BASE_X + BASE_SIZE * 1;
const int X2 = BASE_X + BASE_SIZE * 2;
const int X3 = BASE_X + BASE_SIZE * 3;
const int X4 = BASE_X + BASE_SIZE * 4;
const int X5 = WIDTH - BASE_SIZE * 1;
const int X6 = WIDTH - BASE_SIZE * 0;

const int Y0 = (HEIGHT-(BASE_SIZE*3)) + BASE_SIZE * 0;
const int Y1 = (HEIGHT-(BASE_SIZE*3)) + BASE_SIZE * 1;
const int Y2 = (HEIGHT-(BASE_SIZE*3)) + BASE_SIZE * 2;
const int Y3 = (HEIGHT-(BASE_SIZE*3)) + BASE_SIZE * 3;


const int BUTTON_SIZE = 80;
const int BUTTON_GAP  = 95;

const int BUTTON1_X = 640 + ((800-640)- BUTTON_SIZE) / 2;
const int BUTTON2_X = BUTTON_GAP - BUTTON_SIZE;
const int BUTTON2_Y = 480 - BUTTON_GAP;

TRect HIT_RECT[NUM_KEY] =
{
	// up
	{ BUTTON1_X, 480-BUTTON_GAP*2, BUTTON1_X+BUTTON_SIZE, 480-BUTTON_GAP*2+BUTTON_SIZE },
	// down
	{ BUTTON1_X, 480-BUTTON_GAP*1, BUTTON1_X+BUTTON_SIZE, 480-BUTTON_GAP*1+BUTTON_SIZE },
	// left
	{ BUTTON2_X+BUTTON_GAP*0, BUTTON2_Y, BUTTON2_X+BUTTON_GAP*0+BUTTON_SIZE, BUTTON2_Y+BUTTON_SIZE },
	// right
	{ BUTTON2_X+BUTTON_GAP*1, BUTTON2_Y, BUTTON2_X+BUTTON_GAP*1+BUTTON_SIZE, BUTTON2_Y+BUTTON_SIZE },
	// shoot
	{ BUTTON1_X, 480-BUTTON_GAP*3.5, BUTTON1_X+BUTTON_SIZE, 480-BUTTON_GAP*3.5+BUTTON_SIZE },
	// special
	{ BUTTON1_X, 480-BUTTON_GAP*3.5, BUTTON1_X+BUTTON_SIZE, 480-BUTTON_GAP*3.5+BUTTON_SIZE },
	// sys1
	{ BUTTON1_X, 480-BUTTON_GAP*4.7, BUTTON1_X+BUTTON_SIZE, 480-BUTTON_GAP*4.7+BUTTON_SIZE }
};

#else

const int WIDTH  = 800;
const int HEIGHT = 480;
const int HEIGHT_BASE = 480;

const int X0 = WIDTH * 0 / 3;
const int X1 = WIDTH * 1 / 3;
const int X2 = WIDTH * 2 / 3;
const int X3 = WIDTH * 3 / 3;

const int Y0 = (HEIGHT-HEIGHT_BASE) + HEIGHT_BASE * 0 / 3;
const int Y1 = (HEIGHT-HEIGHT_BASE) + HEIGHT_BASE * 1 / 3;
const int Y2 = (HEIGHT-HEIGHT_BASE) + HEIGHT_BASE * 2 / 3;
const int Y3 = (HEIGHT-HEIGHT_BASE) + HEIGHT_BASE * 3 / 3;

TRect HIT_RECT[NUM_KEY] =
{
	// up
	{ X0, Y0, X3, Y1 },
	// down
	{ X0, Y2, X3, Y3 },
	// left
	{ X0, Y0, X1, Y3 },
	// right
	{ X2, Y0, X3, Y3 },
	// shoot
	{ 0, 0, 0, 0 },
	// special
	{ 0, 0, 0, 0 },
	// sys1
	{ 0, 0, 0, 0 },
//	{ 0, 0, WIDTH, WIDTH*240/320 }
};

#endif

#include <math.h>

TMultiTouchInfoArray s_multi_touch_info_array;

void CInputDevice::UpdateInputState()
{
	bool key_state_saved[NUM_KEY];

	memcpy(key_state_saved, g_key_state, sizeof(key_state_saved));

	for (int i = 0; i < NUM_KEY; i++)
	{
		g_key_state[i] = false;
	}

	Osp::Base::Collection::IList* pList = g_touch->GetTouchInfoListN();

	if (pList)
	{
		const int  RADIUS_MIN = 30;
		const int  RADIUS_MAX = 100;
		const TPos CENTER_POS = {RADIUS_MAX, 480-RADIUS_MAX};

		int ix_touch_info = 0;

		s_multi_touch_info_array[ix_touch_info].x = -1;
		s_multi_touch_info_array[ix_touch_info].y = -1;

		for (int i = 0; i < pList->GetCount(); i++)
		{
			Osp::Ui::TouchInfo* p_touch_info = (Osp::Ui::TouchInfo*)pList->GetAt(i);
			if (p_touch_info && p_touch_info->status == Osp::Ui::TOUCH_PRESSED)
			{
				int x = p_touch_info->position.x;
				int y = p_touch_info->position.y;

				// multi-touch
				s_multi_touch_info_array[ix_touch_info].x = x;
				s_multi_touch_info_array[ix_touch_info].y = y;

				if (++ix_touch_info >= sizeof(s_multi_touch_info_array) / sizeof(s_multi_touch_info_array[0]))
					break;

				s_multi_touch_info_array[ix_touch_info].x = -1;
				s_multi_touch_info_array[ix_touch_info].y = -1;

				// ket mapping
				for (int key = 0; key < NUM_KEY; key++)
				{
					g_key_state[key] |= s_InRect(x, y, HIT_RECT[key]);
				}
			}
		}

		pList->RemoveAll(true);
		delete pList;

		for (int i = 0; i < NUM_KEY; i++)
		{
			if (!key_state_saved[i] && g_key_state[i])
				g_key_pressed[i] = true;
		}
	}
	else
	{
		s_multi_touch_info_array[0].x = -1;
		s_multi_touch_info_array[0].y = -1;
	}

}

bool CInputDevice::IsKeyHeldDown(TInputKey key) const
{
	switch (key)
	{
	case INPUT_KEY_UP:
		return g_key_state[0];
	case INPUT_KEY_DOWN:
		return g_key_state[1];
	case INPUT_KEY_LEFT:
		return g_key_state[2];
	case INPUT_KEY_RIGHT:
		return g_key_state[3];
	case INPUT_KEY_A:
		return g_key_state[4];
	case INPUT_KEY_B:
		return g_key_state[5];
	case INPUT_KEY_SYS1:
		return g_key_state[6];
	default:
		return false;
	}
}

bool CInputDevice::WasKeyPressed(TInputKey key) const
{
	bool has_pressed = false;

	switch (key)
	{
	case INPUT_KEY_UP:
		has_pressed = g_key_pressed[0];
		g_key_pressed[0] = false;
		break;
	case INPUT_KEY_DOWN:
		has_pressed = g_key_pressed[1];
		g_key_pressed[1] = false;
		break;
	case INPUT_KEY_LEFT:
		has_pressed = g_key_pressed[2];
		g_key_pressed[2] = false;
		break;
	case INPUT_KEY_RIGHT:
		has_pressed = g_key_pressed[3];
		g_key_pressed[3] = false;
		break;
	case INPUT_KEY_A:
		has_pressed = g_key_pressed[4];
		g_key_pressed[4] = false;
		break;
	case INPUT_KEY_B:
		has_pressed = g_key_pressed[5];
		g_key_pressed[5] = false;
		break;
	case INPUT_KEY_SYS1:
		has_pressed = g_key_pressed[6];
		g_key_pressed[6] = false;
		break;
	default:
		return false;
	}

	return has_pressed;
}

bool CInputDevice::GetMultiTouchInfo(TMultiTouchInfoArray& multi_touch_info_array) const
{
	if ((s_multi_touch_info_array[0].x < 0) || (s_multi_touch_info_array[0].y < 0))
		return false;

	memcpy(multi_touch_info_array, s_multi_touch_info_array, sizeof(multi_touch_info_array));

	return true;
}

#elif (TARGET_DEVICE == TARGET_TIZEN)

#include <string.h>

using namespace avej_lite;

const int NUM_KEY = 7;

namespace
{
	int s_native_pressed_x = -1;
	int s_native_pressed_y = -1;

	bool g_key_state[NUM_KEY];
	bool g_key_pressed[NUM_KEY];
}

namespace target
{
	void setTouchPos(int x, int y)
	{
		s_native_pressed_x = x;
		s_native_pressed_y = y;
	}
}

CInputDevice::CInputDevice()
{
	for (int i = 0; i < NUM_KEY; i++)
	{
		g_key_state[i] = false;
		g_key_pressed[i] = false;
	}
}

CInputDevice::~CInputDevice()
{
}

struct TPos
{
	int x, y;
};

struct TRect
{
	int x1, y1, x2, y2;
};

bool s_InRect(int x, int y, int x1, int y1, int x2, int y2)
{
	return (x >= x1 && x < x2 && y >= y1 && y < y2);
}

bool s_InRect(int x, int y, const TRect& rect)
{
	return (x >= rect.x1 && x < rect.x2 && y >= rect.y1 && y < rect.y2);
}

bool s_InRange(int x, int y, const TPos& pos, int min, int max)
{
	int diff_x = x - pos.x;
	int diff_y = y - pos.y;

	int distance = diff_x * diff_x + diff_y * diff_y;
	return (distance >= min * min) && (distance >= max * max);
}

const int WIDTH  = 800;
const int HEIGHT = 480;
const int HEIGHT_BASE = 480;

const int X0 = WIDTH * 0 / 3;
const int X1 = WIDTH * 1 / 3;
const int X2 = WIDTH * 2 / 3;
const int X3 = WIDTH * 3 / 3;

const int Y0 = (HEIGHT-HEIGHT_BASE) + HEIGHT_BASE * 0 / 3;
const int Y1 = (HEIGHT-HEIGHT_BASE) + HEIGHT_BASE * 1 / 3;
const int Y2 = (HEIGHT-HEIGHT_BASE) + HEIGHT_BASE * 2 / 3;
const int Y3 = (HEIGHT-HEIGHT_BASE) + HEIGHT_BASE * 3 / 3;

TRect HIT_RECT[NUM_KEY] =
{
	// up
	{ X0, Y0, X3, Y1 },
	// down
	{ X0, Y2, X3, Y3 },
	// left
	{ X0, Y0, X1, Y3 },
	// right
	{ X2, Y0, X3, Y3 },
	// shoot
	{ 0, 0, 0, 0 },
	// special
	{ 0, 0, 0, 0 },
	// sys1
	{ WIDTH - 100, 0, WIDTH, 100 },
//	{ 0, 0, WIDTH, WIDTH*240/320 }
};

TMultiTouchInfoArray s_multi_touch_info_array;

#include <math.h>

void CInputDevice::UpdateInputState()
{
	bool key_state_saved[NUM_KEY];

	memcpy(key_state_saved, g_key_state, sizeof(key_state_saved));

	for (int i = 0; i < NUM_KEY; i++)
	{
		g_key_state[i] = false;
	}

	if (s_native_pressed_x >= 0 && s_native_pressed_y >= 0)
	{
		{
			int x = s_native_pressed_x;
			int y = s_native_pressed_y;

			// key mapping
			for (int key = 0; key < NUM_KEY; key++)
			{
				g_key_state[key] |= s_InRect(x, y, HIT_RECT[key]);
			}
		}

		for (int i = 0; i < NUM_KEY; i++)
		{
			if (!key_state_saved[i] && g_key_state[i])
				g_key_pressed[i] = true;
		}
	}
	else
	{
		s_multi_touch_info_array[0].x = -1;
		s_multi_touch_info_array[0].y = -1;
	}

}

bool CInputDevice::IsKeyHeldDown(TInputKey key) const
{
	switch (key)
	{
	case INPUT_KEY_UP:
		return g_key_state[0];
	case INPUT_KEY_DOWN:
		return g_key_state[1];
	case INPUT_KEY_LEFT:
		return g_key_state[2];
	case INPUT_KEY_RIGHT:
		return g_key_state[3];
	case INPUT_KEY_A:
		return g_key_state[4];
	case INPUT_KEY_B:
		return g_key_state[5];
	case INPUT_KEY_SYS1:
		return g_key_state[6];
	default:
		return false;
	}
}

bool CInputDevice::WasKeyPressed(TInputKey key) const
{
	bool has_pressed = false;

	switch (key)
	{
	case INPUT_KEY_UP:
		has_pressed = g_key_pressed[0];
		g_key_pressed[0] = false;
		break;
	case INPUT_KEY_DOWN:
		has_pressed = g_key_pressed[1];
		g_key_pressed[1] = false;
		break;
	case INPUT_KEY_LEFT:
		has_pressed = g_key_pressed[2];
		g_key_pressed[2] = false;
		break;
	case INPUT_KEY_RIGHT:
		has_pressed = g_key_pressed[3];
		g_key_pressed[3] = false;
		break;
	case INPUT_KEY_A:
		has_pressed = g_key_pressed[4];
		g_key_pressed[4] = false;
		break;
	case INPUT_KEY_B:
		has_pressed = g_key_pressed[5];
		g_key_pressed[5] = false;
		break;
	case INPUT_KEY_SYS1:
		has_pressed = g_key_pressed[6];
		g_key_pressed[6] = false;
		break;
	default:
		return false;
	}

	return has_pressed;
}

bool CInputDevice::GetMultiTouchInfo(TMultiTouchInfoArray& multi_touch_info_array) const
{
	if ((s_multi_touch_info_array[0].x < 0) || (s_multi_touch_info_array[0].y < 0))
		return false;

	memcpy(multi_touch_info_array, s_multi_touch_info_array, sizeof(multi_touch_info_array));

	return true;
}

#else

#error "..."

#endif
