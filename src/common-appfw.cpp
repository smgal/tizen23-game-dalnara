
#include "avejapp_register.h"
#include "avej_lite/avej_util.h"
#include "common-appfw.h"

#include <cassert>
#include <cstring>
#include <string>

namespace avej_lite
{
	namespace util
	{
		void SetAppPath(const string& path);
	}
}

namespace target
{
	void setTouchPos(int x, int y);
}

class AppFwDalnara: public AppFw
{
public:
	struct TouchInfo
	{
		bool is_pressed;
		int x, y;

		TouchInfo()
		: is_pressed(false)
		, x(0)
		, y(0)
		{
		}
	} m_touch_info;

	AppDesc m_app_desc;
	bool m_on_progress;
	TAppCallback m_app_callback;

	AppFwDalnara()
		: m_on_progress(true)
	{
		memset(&m_touch_info, 0, sizeof(m_touch_info));
		memset(&m_app_desc, 0, sizeof(m_app_desc));

		CAppRegister::FnGetApp* p_app = CAppRegister::GetAppByName("DALNARA");
//		CAppRegister::FnGetApp* p_app = CAppRegister::GetAppByName("AvejDream");
//		CAppRegister::FnGetApp* p_app = CAppRegister::GetAppByName("AvejKano");
//		CAppRegister::FnGetApp* p_app = CAppRegister::GetAppByName("ManoEri");
//		CAppRegister::FnGetApp* p_app = CAppRegister::GetAppByName("ShitFighter_Wave");

		assert(p_app);

		bool must_true = (*p_app)(m_app_callback);
		assert(must_true);
	}

	virtual void onCreate(const AppDesc& app_desc)
	{
		memcpy(&m_app_desc, &app_desc, sizeof(m_app_desc));

		avej_lite::util::SetAppPath(app_desc.res_path);

		m_app_callback.Initialize(NULL);
	}

	virtual void onDestroy(void)
	{
		m_app_callback.Finalize();
	}

	virtual void onProcess(const BufferDesc& buffer_desc)
	{
		m_on_progress = m_app_callback.Process();
	}

	virtual void onTouchDown(int x, int y)
	{
		m_touch_info.is_pressed = true;
		m_touch_info.x = x;
		m_touch_info.y = y;

		target::setTouchPos(x, y);
	}

	virtual void onTouchUp(int x, int y)
	{
		m_touch_info.is_pressed = false;

		target::setTouchPos(-1, -1);
	}

	virtual void onTouchMove(int x, int y)
	{
		m_touch_info.is_pressed = true;
		m_touch_info.x = x;
		m_touch_info.y = y;

		target::setTouchPos(x, y);
	}

	virtual bool inTerminating(void)
	{
		return !m_on_progress;
	}
};

AppFw* AppFw::create(void)
{
	static AppFwDalnara s_app_fw_instance;

	return &s_app_fw_instance;
}