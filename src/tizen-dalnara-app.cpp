
#include "tizen-dalnara.h"
#include <math.h>
#include <Elementary_GL_Helpers.h>

#include "common-appfw.h"
#include <cassert>
#include <cstring>

#include <app.h>

/////////////////////////////////////

ELEMENTARY_GLVIEW_GLOBAL_DEFINE()

/////////////////////////////////////

namespace
{
	AppFw* s_app_fw = NULL;
}

namespace app
{
	bool on_ready = false;
	bool initialized = false;

	void _get_size(Evas_Object *obj, int& width, int& height)
	{
		elm_glview_size_get(obj, &width, &height);
	}

	bool _is_ready(Evas_Object *obj)
	{
		int width;
		int height;
		_get_size(obj, width, height);

		return (width > 1) && (height > 1);
	}

	void _init(Evas_Object *obj)
	{
		ELEMENTARY_GLVIEW_GLOBAL_USE(obj);

		s_app_fw = AppFw::create();
		assert(s_app_fw);

		initialized = true;

		app::resize(obj);

		{
			AppFw::AppDesc app_desc;
			{
				char* res_path_buff = app_get_resource_path();
				strcpy(app_desc.res_path, res_path_buff);
				free(res_path_buff);
			}

			s_app_fw->onCreate(app_desc);
		}
	}
}

void app::init(Evas_Object *obj)
{
	if (!on_ready)
	{
		if (_is_ready(obj))
		{
			app::_init(obj);
		}
		on_ready = true;
	}
}

void app::done(Evas_Object *obj)
{
	ELEMENTARY_GLVIEW_GLOBAL_USE(obj);

	assert(s_app_fw);

	s_app_fw->onDestroy();

	// cannot do 'delete s_app_fw'

	s_app_fw = NULL;
}

void app::resize(Evas_Object *obj)
{
	if (!initialized && on_ready)
	{
		if (_is_ready(obj))
		{
			app::_init(obj);
		}
	}
	else
	{
		ELEMENTARY_GLVIEW_GLOBAL_USE(obj);

		int width;
		int height;

		_get_size(obj, width, height);

		glViewport(0, 0, width, height);
	}
}

void app::loop(Evas_Object *obj)
{
	if (initialized)
	{
		ELEMENTARY_GLVIEW_GLOBAL_USE(obj);

		assert(s_app_fw);

		AppFw::BufferDesc buffer_desc;
		memset(&buffer_desc, 0, sizeof(buffer_desc));

		s_app_fw->onProcess(buffer_desc);

		if (s_app_fw->inTerminating())
		{
			ui_app_exit();
		}
	}
}

void app::on_mouse_down(void *data, Evas *e, Evas_Object *eo, void *event)
{
	if (s_app_fw)
	{
		Evas_Event_Mouse_Down* ev = (Evas_Event_Mouse_Down*)event;
		s_app_fw->onTouchDown(ev->canvas.x, ev->canvas.y);
	}
}

void app::on_mouse_up(void *data, Evas *e, Evas_Object *eo, void *event)
{
	if (s_app_fw)
	{
		Evas_Event_Mouse_Up* ev = (Evas_Event_Mouse_Up*)event;
		s_app_fw->onTouchUp(ev->canvas.x, ev->canvas.y);
	}
}

void app::on_mouse_move(void *data, Evas *e, Evas_Object *eo, void *event)
{
	if (s_app_fw)
	{
		Evas_Event_Mouse_Move* ev = (Evas_Event_Mouse_Move*)event;
		s_app_fw->onTouchMove(ev->cur.canvas.x, ev->cur.canvas.y);
	}
}

void app::on_mouse_multi_down(void *data, Evas *e, Evas_Object *eo, void *event)
{
	if (s_app_fw)
	{
		Evas_Event_Multi_Down* ev = (Evas_Event_Multi_Down*)event;
		s_app_fw->onTouchDown(ev->canvas.x, ev->canvas.y);
		// dlog_print(DLOG_ERROR, LOG_TAG, "[SMGAL] multi-point touch down(%i) - (%3i %3i)", ev->device, ev->canvas.x, ev->canvas.y);
	}
}
