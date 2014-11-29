
#include <app.h>
#include <tizen.h>
#include "common-appfw.h"
#include "tizen-dalnara.h"

////////////////////////////////////

#define S(a) evas_object_show(a)

#define SX(a) do { \
	evas_object_size_hint_align_set(a, EVAS_HINT_FILL, EVAS_HINT_FILL); \
	evas_object_size_hint_weight_set(a, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
	evas_object_show(a); \
	} while (0)

#define SF(a) do { \
	evas_object_size_hint_align_set(a, EVAS_HINT_FILL, EVAS_HINT_FILL); \
	evas_object_size_hint_weight_set(a, 0.00001, 0.00001); \
	evas_object_show(a); \
	} while (0)

namespace callback
{
	static void
	win_resize(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
	{
		int w, h;
		appdata_s *ad = (appdata_s *)data;

		evas_object_geometry_get(ad->win, NULL, NULL, &w, &h);
		evas_object_resize(ad->table, w, h);
		evas_object_resize(ad->bg, w, h);
	}

	static void
	win_close(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
	{
		ui_app_exit();
	}

	static Eina_Bool
	anim_process(void *data)
	{
		appdata_s *ad = (appdata_s *)data;

		elm_glview_changed_set(ad->glview);
		return ECORE_CALLBACK_RENEW;
	}

	static void
	anim_destroy(void *data, Evas *evas, Evas_Object *obj, void *event_info)
	{
		Ecore_Animator *ani = (Ecore_Animator *)data;
		ecore_animator_del(ani);
	}
}

static Evas_Object*
_glview_create(appdata_s *ad)
{
	Evas_Object *obj;

	/* Create a GLView with an OpenGL-ES 1.1 context */
	obj = elm_glview_version_add(ad->win, EVAS_GL_GLES_1_X);
	elm_table_pack(ad->table, obj, 1, 1, 3, 1);
	evas_object_data_set(obj, APPDATA_KEY, ad);

	elm_glview_mode_set(obj, Elm_GLView_Mode(ELM_GLVIEW_ALPHA | ELM_GLVIEW_DEPTH));
	elm_glview_resize_policy_set(obj, ELM_GLVIEW_RESIZE_POLICY_RECREATE);
	elm_glview_render_policy_set(obj, ELM_GLVIEW_RENDER_POLICY_ON_DEMAND);

	elm_glview_init_func_set(obj, app::init);
	elm_glview_del_func_set(obj, app::done);
	elm_glview_resize_func_set(obj, app::resize);
	elm_glview_render_func_set(obj, app::loop);

	return obj;
}

static bool
app_create(void *data)
{
	Evas_Object *o, *t;
	appdata_s *ad = (appdata_s*)data;

	/* Force OpenGL engine */
	elm_config_accel_preference_set("opengl");

	/* Add a window */
	ad->win = o = elm_win_add(NULL,"glview", ELM_WIN_BASIC);

	if (elm_win_wm_rotation_supported_get(ad->win))
	{
		int rots[] = { 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), sizeof(rots) / sizeof(rots[0]));
	}

	evas_object_smart_callback_add(o, "delete,request", callback::win_close, ad);
	evas_object_event_callback_add(o, EVAS_CALLBACK_RESIZE, callback::win_resize, ad);
	eext_object_event_callback_add(o, EEXT_CALLBACK_BACK, callback::win_close, ad);

	S(o);

	/* Add a background */
	ad->bg = o = elm_bg_add(ad->win);
	elm_win_resize_object_add(ad->win, ad->bg);
	elm_bg_color_set(o, 68, 68, 68);

	S(o);

	/* Add a resize conformant */
	ad->conform = o = elm_conformant_add(ad->win);
	elm_win_resize_object_add(ad->win, ad->conform);
	SX(o);

	ad->table = t = elm_table_add(ad->win);
	S(t);

	ad->glview = o = _glview_create(ad);
	evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_DOWN, app::on_mouse_down, NULL);
	evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_UP, app::on_mouse_up, NULL);
	evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_MOVE, app::on_mouse_move, NULL);
	evas_object_event_callback_add(o, EVAS_CALLBACK_MULTI_DOWN, app::on_mouse_multi_down, NULL);
	SX(o);

	/* Add an animator to call _anim_cb_() every (1/60) seconds
	 * _anim_cb() indicates that glview has changed, which eventually triggers
	 * function(draw_gl() here) to redraw glview surface
	 */
	ecore_animator_frametime_set(1.0 / 60.0);
	ad->anim = ecore_animator_add(callback::anim_process, ad);
	evas_object_event_callback_add(ad->glview, EVAS_CALLBACK_DEL, callback::anim_destroy, ad->anim);

	return true;
}

extern "C" int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {NULL,};

	event_callback.create = app_create;

	ret = ui_app_main(argc, argv, &event_callback, &ad);

	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "The application failed to start, and returned %d", ret);

	return ret;
}
