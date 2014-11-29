
#ifndef __IQB_CLASS_3D_TEXTURE_H__
#define __IQB_CLASS_3D_TEXTURE_H__

#include "iqb_class_3d.h"

namespace erio
{
	struct CTexture
	{
		IDirect3DDevice9*  m_p_ref_d3d_device;
		IDirect3DTexture9* m_p_texture;

		CTexture(IDirect3DDevice9* p_d3d_device, const char* sz_file_name)
			: m_p_ref_d3d_device(p_d3d_device)
			, m_p_texture(NULL)
		{
			D3DXCreateTextureFromFileA(p_d3d_device, sz_file_name, &m_p_texture);
		}

		~CTexture()
		{
			if (m_p_texture)
				m_p_texture->Release();
		}
	};

} // namespace erio

#endif // #ifndef __IQB_CLASS_3D_TEXTURE_H__
