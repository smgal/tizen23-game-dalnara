
#ifndef __IQB_BASE_GFX_H__
#define __IQB_BASE_GFX_H__

#include "iqb_z_config.h"

namespace erio
{
	////////////////////////////////////////////////////////////////////////////
	// external

	extern avej_lite::IGfxDevice*  g_p_gfx_device;
	extern avej_lite::IGfxSurface* g_p_back_buffer;

	////////////////////////////////////////////////////////////////////////////
	//

	namespace gfx
	{
		void Init(void);
		void SetFlatMode(void);

		void FillRect(unsigned long back_color, int x, int y, int width, int height);
		void BitBlt(int x_dest, int y_dest, avej_lite::IGfxSurface* p_surface, int x_sour, int y_sour, int w_sour, int h_sour);
		void BlendBlt(int x_dest, int y_dest, avej_lite::IGfxSurface* p_surface, int x_sour, int y_sour, int w_sour, int h_sour, float opacity);
		void DrawText(int x, int y, const char* string);
	}

} // namespace erio

#endif // #ifndef __IQB_BASE_GFX_H__
