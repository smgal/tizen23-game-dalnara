
#ifndef __TIZNE_DALNARA_H__
#define __TIZNE_DALNARA_H__

#include <Evas_GL.h>
#include <Elementary.h>
#include <efl_extension.h>
#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "tizen-dalnara"

#define APPDATA_KEY "AppData"

struct _appdata_s
{
   Evas_Object *table, *bg;
   Evas_Object *win;
   Evas_Object *glview;
   Ecore_Animator *anim;
   Evas_Object *conform;
};
typedef struct _appdata_s appdata_s;

namespace app
{
	void init(Evas_Object *obj);
	void done(Evas_Object *obj);
	void resize(Evas_Object *obj);
	void loop(Evas_Object *obj);
	void on_mouse_down(void *data, Evas *e, Evas_Object *eo, void *event);
	void on_mouse_up(void *data, Evas *e, Evas_Object *eo, void *event);
	void on_mouse_move(void *data, Evas *e, Evas_Object *eo, void *event);
	void on_mouse_multi_down(void *data, Evas *e, Evas_Object *eo, void *event);
}

#endif // __TIZNE_DALNARA_H__
