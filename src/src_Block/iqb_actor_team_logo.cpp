
#include "iqb_z_config.h"
#include "iqb_base_gfx.h"

#include "iqb_class_3d_texture.h"

using namespace avej_lite;

namespace erio
{
	extern IDirect3DDevice9* g_p_d3d_device;

namespace team_logo
{
	const struct
	{
		int x;
		int y;
		int width;
		int height;
	} LOGO_RECT =
	{
		0,
		0,
		270,
		178
	};

	float alpha_constant = 0.2f;
	iu::shared_ptr<CTexture> sprite;

	bool OnCreate(unsigned long param)
	{
		gfx::SetFlatMode();

		sprite = iu::shared_ptr<CTexture>(new CTexture(g_p_d3d_device, "team_dalnara_logo.tga"));
		if (sprite->m_p_texture == 0)
		{
			sprite = iu::shared_ptr<CTexture>(new CTexture(g_p_d3d_device, "res_Block/team_dalnara_logo.tga"));
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
		static long startTick = util::GetTicks();
		static bool increase = true;
		long currTick = util::GetTicks();

		CInputDevice& input_device = avej_lite::singleton<CInputDevice>::get();
		input_device.UpdateInputState();

		if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_SYS1) || alpha_constant <= 0.f)
		{
			g_ChangeState(STATE_TITLE, 0x00010001);
			return false;
		}
		
		{
			IGfxDevice::TDeviceDesc screenDesc;
			g_p_gfx_device->GetDesc(screenDesc);

			IGfxDevice::TDeviceDesc device_desc;
			g_p_gfx_device->GetDesc(device_desc);

			const int dest_x = (device_desc.width - LOGO_RECT.width) / 2;
			const int dest_y = (device_desc.height - LOGO_RECT.height) / 2;

			g_p_gfx_device->BeginDraw();
			{
				gfx::BlendBlt(dest_x, dest_y, sprite->m_p_texture->m_p_surface, LOGO_RECT.x, LOGO_RECT.y, LOGO_RECT.width, LOGO_RECT.height, alpha_constant);
			}
			g_p_gfx_device->EndDraw();
			g_p_gfx_device->Flip();

			if (alpha_constant < 1.0f && increase)
			{
				alpha_constant += 0.005f;
				if (alpha_constant >= 1.f)
				{
					increase = false;
				}
			}
			else if (currTick - startTick >= 5000)
			{
				alpha_constant -= 0.005f;
			}
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
