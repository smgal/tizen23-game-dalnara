
////////////////////////////////////////////////////////////////////////////////
// uses

#include "iqb_z_config.h"

#include "iqb_base_gfx.h"

#include "iqb_class_3d_texture.h"
#include "iqb_class_3d_attrib.h"


using namespace avej_lite;

namespace erio
{
	extern IDirect3DDevice9* g_p_d3d_device;

namespace title
{
	// alpha constant
	const float ALPHA_STEP = 0.005f;

	float alpha_constant = 0.0f;
	float alpha_advance = ALPHA_STEP;

	// translucent rectangle
	int rectangle_x = 0;
	int rectangle_y = 300;

	iu::shared_ptr<CTexture> sprite;

	bool OnCreate(unsigned long param)
	{
		gfx::SetFlatMode();

		sprite = iu::shared_ptr<CTexture>(new CTexture(g_p_d3d_device, "team_logo.tga"));
		if (sprite->m_p_texture == 0)
		{
			sprite = iu::shared_ptr<CTexture>(new CTexture(g_p_d3d_device, "res_Block/team_logo.tga"));
		}

		return true;
	}

	bool OnDestory(void)
	{
		sprite.setNull();

		return true;
	}

	bool OnProcess(void)
	{
		CInputDevice& input_device = avej_lite::singleton<CInputDevice>::get();
		input_device.UpdateInputState();

		if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_SYS1))
		{
			g_ChangeState(STATE_GAME_PLAY, 0x00010001);
			return false;
		}

		{
			IGfxDevice::TDeviceDesc screenDesc;
			g_p_gfx_device->GetDesc(screenDesc);

			g_p_gfx_device->BeginDraw();

			gfx::BlendBlt(0, 0, sprite->m_p_texture->m_p_surface, 0, 0, 660, 480, alpha_constant);
			gfx::FillRect(0x80FF4080, rectangle_x, rectangle_y, 100, 100); 

			// revise alpha constant
			{
				alpha_constant += alpha_advance;
				if (alpha_constant >= 1.0f)
					alpha_advance = -ALPHA_STEP;
				if (alpha_constant <= 0.0f)
					alpha_advance = +ALPHA_STEP;
			}

			rectangle_x = (rectangle_x++ > 500) ? 0 : rectangle_x;

			g_p_gfx_device->EndDraw();
			g_p_gfx_device->Flip();
		}

		return true;
	}

	AppCallback callback =
	{
		OnCreate,
		OnDestory,
		OnProcess
	};
}

} // namespace erio
