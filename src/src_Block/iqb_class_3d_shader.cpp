
#include "iqb_class_3d_shader.h"

iu::shared_ptr<erio::TVertexShader> erio::CreateVertexShader(IDirect3DDevice9* p_d3d_device, const char* sz_shader_code, DWORD fvf)
{
	iu::shared_ptr<TVertexShader> vertexShader = iu::shared_ptr<TVertexShader>(new TVertexShader);
/*
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(fvf, decl);

	if (FAILED(p_d3d_device->CreateVertexDeclaration(decl, &vertexShader->p_declation)))
		return iu::shared_ptr<TVertexShader>();

	{
		LPD3DXBUFFER pCode;

		if (FAILED(D3DXAssembleShader(sz_shader_code, strlen(sz_shader_code), NULL, NULL, D3DXSHADER_DEBUG, &pCode, NULL)))
			return iu::shared_ptr<TVertexShader>();

		p_d3d_device->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), &vertexShader->p_shader);

		pCode->Release();
	}
*/
	return vertexShader;
}

iu::shared_ptr<erio::TPixelShader> erio::CreatePixelShader(IDirect3DDevice9* p_d3d_device, const char* sz_shader_code)
{
	iu::shared_ptr<TPixelShader> pixelShader = iu::shared_ptr<TPixelShader>(new TPixelShader);
/*
	{
		LPD3DXBUFFER pCode;

		if (FAILED(D3DXAssembleShader(sz_shader_code, strlen(sz_shader_code), NULL, NULL, D3DXSHADER_DEBUG, &pCode, NULL)))
			return iu::shared_ptr<TPixelShader>();

		p_d3d_device->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), &pixelShader->p_shader);

		pCode->Release();
	}
*/	
	return pixelShader;
}

void erio::shader::SetFog(IDirect3DDevice9* p_d3d_device, float fog_start, float fog_end)
{
	float fogData[4] = { fog_start, fog_end, 1.0f / (fog_end - fog_start), 0.0f };
	p_d3d_device->SetVertexShaderConstantF(3, fogData, 1);
}

void erio::shader::SetTexFactor(IDirect3DDevice9* p_d3d_device, float factor)
{
	float textureFactor[4] = { 1.0f, 1.0f, 1.0f, factor };
	p_d3d_device->SetVertexShaderConstantF(23, textureFactor, 1);
}

void erio::shader::SetLightDir(IDirect3DDevice9* p_d3d_device, float x_dir, float y_dir, float z_dir)
{
	float direction[4] = { -x_dir, -y_dir, -z_dir, 0.0f };
	p_d3d_device->SetVertexShaderConstantF(20, direction, 1);
}

void erio::shader::SetLightDiffuse(IDirect3DDevice9* p_d3d_device, float R, float G, float B)
{
	float diffuse[4] = { R, G, B, 0.0f };
	p_d3d_device->SetVertexShaderConstantF(21, diffuse, 1);
}

void erio::shader::SetLightAmbient(IDirect3DDevice9* p_d3d_device, float R, float G, float B)
{
	float ambient[4] = { R, G, B, 0.0f };
	p_d3d_device->SetVertexShaderConstantF(22, ambient, 1);
}
