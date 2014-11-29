
#ifndef __IQB_CLASS_3D_SHADER_H__
#define __IQB_CLASS_3D_SHADER_H__

#include "iqb_class_3d.h"

namespace erio
{
	struct TVertexShader
	{
		IDirect3DVertexDeclaration9* p_declation;
		IDirect3DVertexShader9*      p_shader;

		TVertexShader()
			: p_declation(NULL)
			, p_shader(NULL)
		{
		}

		TVertexShader(IDirect3DVertexDeclaration9* _p_declation, IDirect3DVertexShader9* _p_shader)
			: p_declation(_p_declation)
			, p_shader(_p_shader)
		{
		}

		~TVertexShader()
		{
			if (p_declation)
				p_declation->Release();
			if (p_shader)
				p_shader->Release();
		}
	};

	struct TPixelShader
	{
		IDirect3DPixelShader9* p_shader;

		TPixelShader()
			: p_shader(NULL)
		{
		}

		TPixelShader(IDirect3DPixelShader9* _p_shader)
			: p_shader(_p_shader)
		{
		}

		~TPixelShader()
		{
			if (p_shader)
				p_shader->Release();
		}
	};

	struct TShaderSet
	{
		iu::shared_ptr<TVertexShader> vertex;
		iu::shared_ptr<TPixelShader>  pixel;

		TShaderSet()
		{
		}

		TShaderSet(iu::shared_ptr<TVertexShader> _vertex, iu::shared_ptr<TPixelShader> _pixel)
			: vertex(_vertex)
			, pixel(_pixel)
		{
		}
	};

	iu::shared_ptr<TPixelShader> CreatePixelShader(IDirect3DDevice9* p_d3d_device, const char* sz_shader_code);
	iu::shared_ptr<TVertexShader> CreateVertexShader(IDirect3DDevice9* p_d3d_device, const char* sz_shader_code, DWORD fvf);

	namespace shader
	{
		void SetFog(IDirect3DDevice9* p_d3d_device, float fog_start, float fog_end);
		void SetTexFactor(IDirect3DDevice9* p_d3d_device, float factor);
		void SetLightDir(IDirect3DDevice9* p_d3d_device, float x_dir, float y_dir, float z_dir);
		void SetLightDiffuse(IDirect3DDevice9* p_d3d_device, float R, float G, float B);
		void SetLightAmbient(IDirect3DDevice9* p_d3d_device, float R, float G, float B);
	}

} // namespace erio

#endif // #ifndef __IQB_CLASS_3D_SHADER_H__
