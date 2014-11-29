
#include "iqb_class_3d.h"
#include <math.h>

namespace
{
	void ConvertColor(DWORD in_color, float (&out_color)[4])
	{
		out_color[0] = ((in_color >> 16) & 0xFF) * 1.0f / 255.0f;
		out_color[1] = ((in_color >>  8) & 0xFF) * 1.0f / 255.0f;
		out_color[2] = ((in_color >>  0) & 0xFF) * 1.0f / 255.0f;
		out_color[3] = ((in_color >> 24) & 0xFF) * 1.0f / 255.0f;
	}

	float ConvertFloat(DWORD in_value)
	{
		return *((float*)&in_value);
	}
}

////////////////////////////////////////////////////////////////////////////////
// IDirect3DDevice9

IDirect3DDevice9::IDirect3DDevice9(avej_lite::IGfxDevice* p_gfx_device)
: m_p_gfx_device(p_gfx_device)
{
	memset(&m_transform_matrix, 0, sizeof(m_transform_matrix));
	memset(&m_vertex_shader_constant, 0, sizeof(m_vertex_shader_constant));
	memset(&m_status, 0, sizeof(m_status));

	m_status.texture_factor = 0xFFFFFFFF;
}

_HRESULT IDirect3DDevice9::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
	assert(ppVertexBuffer);
	assert(Usage == D3DUSAGE_WRITEONLY);
	assert(Pool == D3DPOOL_MANAGED);

	*ppVertexBuffer = new IDirect3DVertexBuffer9;
	
	(*ppVertexBuffer)->m_p_buffer = new char[Length];
	(*ppVertexBuffer)->m_fvf = FVF;

	return 0;
}

_HRESULT IDirect3DDevice9::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
	assert(ppIndexBuffer);
	assert(Usage == D3DUSAGE_WRITEONLY);
	assert(Format == D3DFMT_INDEX16);
	assert(Pool == D3DPOOL_MANAGED);

	*ppIndexBuffer = new IDirect3DIndexBuffer9;
	
	(*ppIndexBuffer)->m_p_buffer = new char[Length];

	return 0;
}

_HRESULT IDirect3DDevice9::CreateVertexDeclaration(const D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
	assert(0);
	return 0;
}

_HRESULT IDirect3DDevice9::BeginScene(void)
{
	assert(m_p_gfx_device);

	m_p_gfx_device->BeginDraw(false);

	return 0;
}

_HRESULT IDirect3DDevice9::EndScene(void)
{
	assert(m_p_gfx_device);

	m_p_gfx_device->EndDraw();

	return 0;
}

_HRESULT IDirect3DDevice9::Present(const RECT* pSourceRect, const RECT* pDestRect, void* hDestWindowOverride, const void* pDirtyRegion)
{
	assert(m_p_gfx_device);

	m_p_gfx_device->Flip();

	return 0;
}

void IDirect3DDevice9::Clear(DWORD Count, const D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	//?? color buffer only
	float clear_color[4];

	ConvertColor(Color, clear_color);

	glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);

	GLbitfield gl_clear_flag = 0;

	gl_clear_flag |= (Flags & D3DCLEAR_TARGET) ? GL_COLOR_BUFFER_BIT : 0;
	gl_clear_flag |= (Flags & D3DCLEAR_ZBUFFER) ? GL_DEPTH_BUFFER_BIT : 0;
	gl_clear_flag |= (Flags & D3DCLEAR_STENCIL) ? GL_STENCIL_BUFFER_BIT : 0;

	glClear(gl_clear_flag);
}

void IDirect3DDevice9::SetTexture(DWORD stage, IDirect3DTexture9* p_texture)
{
	assert(stage < TStatus::MAX_STAGE);

	m_status.p_current_texture[stage] = p_texture;
}

void IDirect3DDevice9::SetStreamSource(DWORD StreamNumber, IDirect3DVertexBuffer9* pStreamData, DWORD OffsetInBytes, DWORD Stride)
{
	assert(StreamNumber < TStatus::MAX_STREAM_SOURCE);
	assert(OffsetInBytes == 0);

	m_status.stream_source[StreamNumber].p_stream_data = pStreamData;
	m_status.stream_source[StreamNumber].offset = OffsetInBytes;
	m_status.stream_source[StreamNumber].stride = Stride;
}

void IDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
}

void IDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* p_shader)
{
}

void IDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* p_shader)
{
}

void IDirect3DDevice9::SetFVF(DWORD FVF)
{
	m_status.fvf = FVF;
}

void IDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* p_index_buffer)
{
	m_status.p_index_buffer = p_index_buffer;
}

void IDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE state, const D3DMATRIX* p_matrix)
{
	assert(p_matrix);

	switch (state)
	{
	case D3DTS_VIEW:
		memcpy(&m_transform_matrix.view, p_matrix, sizeof(m_transform_matrix.view));

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glLoadMatrixf(m_transform_matrix.view.f);
		glPushMatrix();

		return;
	case D3DTS_PROJECTION:
		memcpy(&m_transform_matrix.projection, p_matrix, sizeof(m_transform_matrix.projection));

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(m_transform_matrix.projection.f);

		return;
	case D3DTS_WORLD:
		memcpy(&m_transform_matrix.world, p_matrix, sizeof(m_transform_matrix.world));

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glPushMatrix();
		glMultMatrixf(m_transform_matrix.world.f);

		return;
	default:
		assert(false);
	}
}

void IDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE state, D3DMATRIX* p_matrix)
{
	assert(p_matrix);

	switch (state)
	{
	case D3DTS_VIEW:
		memcpy(p_matrix, &m_transform_matrix.view, sizeof(*p_matrix));
		return;
	case D3DTS_PROJECTION:
		memcpy(p_matrix, &m_transform_matrix.projection, sizeof(*p_matrix));
		return;
	case D3DTS_WORLD:
		memcpy(p_matrix, &m_transform_matrix.world, sizeof(*p_matrix));
		return;
	default:
		assert(false);
	}
}

void IDirect3DDevice9::SetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE state_type, DWORD value)
{
	switch (state_type)
	{
	case D3DTSS_ALPHAARG2:
		{
			switch (value)
			{
			case D3DTA_TFACTOR:
				{
					float color[4];

					ConvertColor(m_status.texture_factor, color);
				
					glColor4f(color[0], color[1], color[2], color[3]);
				}
				break;
			case D3DTA_DIFFUSE:
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			default:
				assert(false);
			}
		}
		break;
	default:
		assert(false);
		break;
	}
}

void IDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE state, DWORD value)
{
	switch (state)
	{
	case D3DRS_LIGHTING:
		if (value)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
		break;
	case D3DRS_AMBIENT:
		{
			GLfloat ambient[4];
			ConvertColor(value, ambient);
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
		}
		break;

	case D3DRS_FOGCOLOR:
		{
			GLfloat fog[4];
			ConvertColor(value, fog);
			glFogfv(GL_FOG_COLOR, fog);
		}
		break;
	case D3DRS_FOGENABLE:
		if (value)
			glEnable(GL_FOG);
		else
			glDisable(GL_FOG);
		break;
	case D3DRS_FOGVERTEXMODE:
		if (value == D3DFOG_LINEAR)
		{
			glFogf(GL_FOG_MODE, GL_LINEAR);
		}
		else
		{
			assert(false);
		}
		break;
	case D3DRS_FOGSTART:
		glFogf(GL_FOG_START, ConvertFloat(value));
		break;
	case D3DRS_FOGEND:
		glFogf(GL_FOG_END, ConvertFloat(value));
		break;
	case D3DRS_RANGEFOGENABLE:
		//?? m_p_d3d_device->SetRenderState(D3DRS_RANGEFOGENABLE, 1);
		break;

	case D3DRS_ALPHABLENDENABLE:
		if (value)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
		break;
	case D3DRS_ZENABLE:
		if (value)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		break;
	case D3DRS_TEXTUREFACTOR:
		m_status.texture_factor = value;
		break;
	default:
		assert(false);
		break;
	}
}

void IDirect3DDevice9::LightEnable(DWORD Index, bool Enable)
{
	if (Enable)
		glEnable(GL_LIGHT0 + Index);
	else
		glDisable(GL_LIGHT0 + Index);
}

void IDirect3DDevice9::SetLight(DWORD Index, const D3DLIGHT9* p_light)
{
	if (p_light)
	{
		switch (p_light->Type)
		{
		case D3DLIGHT_DIRECTIONAL:
			{
				GLfloat light_pos[] = { -p_light->Direction.x, -p_light->Direction.y, -p_light->Direction.z, 0.0f };
				glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

				GLfloat light_ambient[] = { p_light->Ambient.r, p_light->Ambient.g, p_light->Ambient.b, p_light->Ambient.a };
				glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

				GLfloat light_diffuse[] = { p_light->Diffuse.r, p_light->Diffuse.g, p_light->Diffuse.b, p_light->Diffuse.a };
				glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
			}
			break;
		default:
			assert(false);
		}
	}

}

void IDirect3DDevice9::SetVertexShaderConstantF(UINT ix_register, const float* p_constant_data, UINT num_vec4)
{
	switch (ix_register)
	{
	case  3: // { fog start, fog end, 1 / (fog end - fog start) }
	case  4: // WVP
	case  8: // WV
	case 12: // W
	case 16: // VP
	case 20: // light direction
	case 21: // light diffuse color
	case 22: // ambient color
		break;
	case 23: // texture factor
		// not used
		break;
	default:
		assert(false);
		break;
	}

	assert(ix_register < MAX_VERTEX_SHADER_CONSTANT);

	memcpy(m_vertex_shader_constant[ix_register], p_constant_data, num_vec4 * sizeof(float) * 4);
}

void IDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE primitive_type, UINT start_vertex, UINT num_primitive)
{
	switch (primitive_type)
	{
	case D3DPT_TRIANGLELIST:
		glDrawArrays(GL_TRIANGLES, start_vertex * 3, num_primitive * 3);
		break;
	case D3DPT_TRIANGLESTRIP:
		glDrawArrays(GL_TRIANGLE_STRIP, start_vertex, num_primitive * 3);
		break;
	default:
		assert(false);
		break;
	}
}

void IDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE primitive_type, INT ix_base_vertex, UINT ix_min_vertex, UINT num_vertices, UINT ix_start, UINT num_primitive)
{
	assert(m_status.p_index_buffer);

	switch (primitive_type)
	{
	case D3DPT_TRIANGLELIST:
		{
			const int MAX_PRIMITIVE = 50;

			assert(num_primitive <= MAX_PRIMITIVE);

			static short s_indice[MAX_PRIMITIVE][3];

			{
				short* p_src = (short*)m_status.p_index_buffer->m_p_buffer;
				short* p_dst = &s_indice[0][0];
				
				for (int i = 0; i < num_primitive * 3; i++)
					*p_dst++ = *p_src++ + ix_base_vertex;
			}

			glDrawElements(GL_TRIANGLES, num_primitive*3, GL_UNSIGNED_SHORT, s_indice);
		}
		break;
	default:
		assert(false);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// IDirect3DVertexBuffer9

IDirect3DVertexBuffer9::IDirect3DVertexBuffer9()
: m_p_buffer(0), m_fvf(0)
{
}

IDirect3DVertexBuffer9::~IDirect3DVertexBuffer9()
{
	delete[] m_p_buffer;
}

_HRESULT IDirect3DVertexBuffer9::Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
	assert(ppbData);
	*ppbData = (void*)m_p_buffer;

	return 0;
}

void IDirect3DVertexBuffer9::Unlock(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// IDirect3DIndexBuffer9

IDirect3DIndexBuffer9::IDirect3DIndexBuffer9()
: m_p_buffer(0)
{
}

IDirect3DIndexBuffer9::~IDirect3DIndexBuffer9()
{
	delete[] m_p_buffer;
}

_HRESULT IDirect3DIndexBuffer9::Lock(UINT OffsetToLock ,UINT SizeToLock, void** ppbData, DWORD Flags)
{
	assert(ppbData);
	*ppbData = (void*)m_p_buffer;

	return 0;
}

void IDirect3DIndexBuffer9::Unlock(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// IDirect3DTexture9

IDirect3DTexture9::IDirect3DTexture9()
	: m_p_surface(0)
{
}

IDirect3DTexture9::~IDirect3DTexture9()
{
	delete m_p_surface;
}

void IDirect3DTexture9::GetLevelDesc(DWORD level, D3DSURFACE_DESC* p_desc)
{
	if (p_desc && m_p_surface)
	{
		//?? m_surface로 받아야 할텐데
		p_desc->Width  = 512;
		p_desc->Height = 256;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Utility

void D3DXCreateTextureFromFileA(IDirect3DDevice9* p_d3d_device, const char* sz_file_name, IDirect3DTexture9** ppTexture)
{
	assert(p_d3d_device);
	assert(p_d3d_device->m_p_gfx_device);
	assert(ppTexture);

	avej_lite::IGfxSurface* p_res_sprite = 0;
	
	if (!p_d3d_device->m_p_gfx_device->CreateSurfaceFrom(sz_file_name, &p_res_sprite))
		return; //??

	*ppTexture = new IDirect3DTexture9;

	(*ppTexture)->m_p_surface = p_res_sprite;
}

////////////////////////////////////////////////////////////////////////////////
// Matrix

const bool IS_ROTATED = false;

void D3DXMatrixIdentity(TD3DMatrix* p_out_mat)
{
	p_out_mat->f[ 0] = 1.0f; p_out_mat->f[ 4] = 0.0f; p_out_mat->f[ 8] = 0.0f; p_out_mat->f[12] = 0.0f;
	p_out_mat->f[ 1] = 0.0f; p_out_mat->f[ 5] = 1.0f; p_out_mat->f[ 9] = 0.0f; p_out_mat->f[13] = 0.0f;
	p_out_mat->f[ 2] = 0.0f; p_out_mat->f[ 6] = 0.0f; p_out_mat->f[10] = 1.0f; p_out_mat->f[14] = 0.0f;
	p_out_mat->f[ 3] = 0.0f; p_out_mat->f[ 7] = 0.0f; p_out_mat->f[11] = 0.0f; p_out_mat->f[15] = 1.0f;
}

void D3DXMatrixScaling(TD3DMatrix* p_out_mat, float x, float y, float z)
{
	p_out_mat->f[ 0] = x;    p_out_mat->f[ 4] = 0.0f; p_out_mat->f[ 8] = 0.0f; p_out_mat->f[12] = 0.0f;
	p_out_mat->f[ 1] = 0.0f; p_out_mat->f[ 5] = y;    p_out_mat->f[ 9] = 0.0f; p_out_mat->f[13] = 0.0f;
	p_out_mat->f[ 2] = 0.0f; p_out_mat->f[ 6] = 0.0f; p_out_mat->f[10] = z;    p_out_mat->f[14] = 0.0f;
	p_out_mat->f[ 3] = 0.0f; p_out_mat->f[ 7] = 0.0f; p_out_mat->f[11] = 0.0f; p_out_mat->f[15] = 1.0f;
}

void D3DXMatrixMultiply(TD3DMatrix* p_out_mat, const TD3DMatrix* p_mat1, const TD3DMatrix* p_mat2)
{
	TD3DMatrix out_mat;

	out_mat.f[ 0] = p_mat1->f[ 0]*p_mat2->f[ 0] + p_mat1->f[ 1]*p_mat2->f[ 4] + p_mat1->f[ 2]*p_mat2->f[ 8] + p_mat1->f[ 3]*p_mat2->f[12];
	out_mat.f[ 1] = p_mat1->f[ 0]*p_mat2->f[ 1] + p_mat1->f[ 1]*p_mat2->f[ 5] + p_mat1->f[ 2]*p_mat2->f[ 9] + p_mat1->f[ 3]*p_mat2->f[13];
	out_mat.f[ 2] = p_mat1->f[ 0]*p_mat2->f[ 2] + p_mat1->f[ 1]*p_mat2->f[ 6] + p_mat1->f[ 2]*p_mat2->f[10] + p_mat1->f[ 3]*p_mat2->f[14];
	out_mat.f[ 3] = p_mat1->f[ 0]*p_mat2->f[ 3] + p_mat1->f[ 1]*p_mat2->f[ 7] + p_mat1->f[ 2]*p_mat2->f[11] + p_mat1->f[ 3]*p_mat2->f[15];

	out_mat.f[ 4] = p_mat1->f[ 4]*p_mat2->f[ 0] + p_mat1->f[ 5]*p_mat2->f[ 4] + p_mat1->f[ 6]*p_mat2->f[ 8] + p_mat1->f[ 7]*p_mat2->f[12];
	out_mat.f[ 5] = p_mat1->f[ 4]*p_mat2->f[ 1] + p_mat1->f[ 5]*p_mat2->f[ 5] + p_mat1->f[ 6]*p_mat2->f[ 9] + p_mat1->f[ 7]*p_mat2->f[13];
	out_mat.f[ 6] = p_mat1->f[ 4]*p_mat2->f[ 2] + p_mat1->f[ 5]*p_mat2->f[ 6] + p_mat1->f[ 6]*p_mat2->f[10] + p_mat1->f[ 7]*p_mat2->f[14];
	out_mat.f[ 7] = p_mat1->f[ 4]*p_mat2->f[ 3] + p_mat1->f[ 5]*p_mat2->f[ 7] + p_mat1->f[ 6]*p_mat2->f[11] + p_mat1->f[ 7]*p_mat2->f[15];

	out_mat.f[ 8] = p_mat1->f[ 8]*p_mat2->f[ 0] + p_mat1->f[ 9]*p_mat2->f[ 4] + p_mat1->f[10]*p_mat2->f[ 8] + p_mat1->f[11]*p_mat2->f[12];
	out_mat.f[ 9] = p_mat1->f[ 8]*p_mat2->f[ 1] + p_mat1->f[ 9]*p_mat2->f[ 5] + p_mat1->f[10]*p_mat2->f[ 9] + p_mat1->f[11]*p_mat2->f[13];
	out_mat.f[10] = p_mat1->f[ 8]*p_mat2->f[ 2] + p_mat1->f[ 9]*p_mat2->f[ 6] + p_mat1->f[10]*p_mat2->f[10] + p_mat1->f[11]*p_mat2->f[14];
	out_mat.f[11] = p_mat1->f[ 8]*p_mat2->f[ 3] + p_mat1->f[ 9]*p_mat2->f[ 7] + p_mat1->f[10]*p_mat2->f[11] + p_mat1->f[11]*p_mat2->f[15];

	out_mat.f[12] = p_mat1->f[12]*p_mat2->f[ 0] + p_mat1->f[13]*p_mat2->f[ 4] + p_mat1->f[14]*p_mat2->f[ 8] + p_mat1->f[15]*p_mat2->f[12];
	out_mat.f[13] = p_mat1->f[12]*p_mat2->f[ 1] + p_mat1->f[13]*p_mat2->f[ 5] + p_mat1->f[14]*p_mat2->f[ 9] + p_mat1->f[15]*p_mat2->f[13];
	out_mat.f[14] = p_mat1->f[12]*p_mat2->f[ 2] + p_mat1->f[13]*p_mat2->f[ 6] + p_mat1->f[14]*p_mat2->f[10] + p_mat1->f[15]*p_mat2->f[14];
	out_mat.f[15] = p_mat1->f[12]*p_mat2->f[ 3] + p_mat1->f[13]*p_mat2->f[ 7] + p_mat1->f[14]*p_mat2->f[11] + p_mat1->f[15]*p_mat2->f[15];

	*p_out_mat = out_mat;
}

void D3DXMatrixTranspose(TD3DMatrix* p_out_mat, const TD3DMatrix* p_mat)
{
	TD3DMatrix out_mat;

	out_mat.f[ 0] = p_mat->f[ 0]; out_mat.f[ 4] = p_mat->f[ 1]; out_mat.f[ 8] = p_mat->f[ 2]; out_mat.f[12] = p_mat->f[ 3];
	out_mat.f[ 1] = p_mat->f[ 4]; out_mat.f[ 5] = p_mat->f[ 5]; out_mat.f[ 9] = p_mat->f[ 6]; out_mat.f[13] = p_mat->f[ 7];
	out_mat.f[ 2] = p_mat->f[ 8]; out_mat.f[ 6] = p_mat->f[ 9]; out_mat.f[10] = p_mat->f[10]; out_mat.f[14] = p_mat->f[11];
	out_mat.f[ 3] = p_mat->f[12]; out_mat.f[ 7] = p_mat->f[13]; out_mat.f[11] = p_mat->f[14]; out_mat.f[15] = p_mat->f[15];

	*p_out_mat = out_mat;
}

void D3DXMatrixTranslation(TD3DMatrix* p_out_mat, float x, float y, float z)
{
	p_out_mat->f[ 0] = 1.0f; p_out_mat->f[ 4] = 0.0f; p_out_mat->f[ 8] = 0.0f; p_out_mat->f[12] = x;
	p_out_mat->f[ 1] = 0.0f; p_out_mat->f[ 5] = 1.0f; p_out_mat->f[ 9] = 0.0f; p_out_mat->f[13] = y;
	p_out_mat->f[ 2] = 0.0f; p_out_mat->f[ 6] = 0.0f; p_out_mat->f[10] = 1.0f; p_out_mat->f[14] = z;
	p_out_mat->f[ 3] = 0.0f; p_out_mat->f[ 7] = 0.0f; p_out_mat->f[11] = 0.0f; p_out_mat->f[15] = 1.0f;
}

void D3DXMatrixRotationX(TD3DMatrix* p_out_mat, float angle)
{
	float f_cos = (float)cos(angle);
	float f_sin = (float)sin(angle);

	p_out_mat->f[ 0] = 1.0f; p_out_mat->f[ 4] = 0.0f;  p_out_mat->f[ 8] = 0.0f;  p_out_mat->f[12] = 0.0f;
	p_out_mat->f[ 1] = 0.0f; p_out_mat->f[ 5] = f_cos; p_out_mat->f[ 9] = f_sin; p_out_mat->f[13] = 0.0f;
	p_out_mat->f[ 2] = 0.0f; p_out_mat->f[ 6] =-f_sin; p_out_mat->f[10] = f_cos; p_out_mat->f[14] = 0.0f;
	p_out_mat->f[ 3] = 0.0f; p_out_mat->f[ 7] = 0.0f;  p_out_mat->f[11] = 0.0f;  p_out_mat->f[15] = 1.0f;
}

void D3DXMatrixRotationY(TD3DMatrix* p_out_mat, float angle)
{
	float f_cos = (float)cos(angle);
	float f_sin = (float)sin(angle);

	p_out_mat->f[ 0] = f_cos; p_out_mat->f[ 4] = 0.0f; p_out_mat->f[ 8] =-f_sin; p_out_mat->f[12] = 0.0f;
	p_out_mat->f[ 1] = 0.0f;  p_out_mat->f[ 5] = 1.0f; p_out_mat->f[ 9] = 0.0f;  p_out_mat->f[13] = 0.0f;
	p_out_mat->f[ 2] = f_sin; p_out_mat->f[ 6] = 0.0f; p_out_mat->f[10] = f_cos; p_out_mat->f[14] = 0.0f;
	p_out_mat->f[ 3] = 0.0f;  p_out_mat->f[ 7] = 0.0f; p_out_mat->f[11] = 0.0f;  p_out_mat->f[15] = 1.0f;
}

void D3DXMatrixRotationZ(TD3DMatrix* p_out_mat, float angle)
{
	float f_cos = (float)cos(angle);
	float f_sin = (float)sin(angle);

	p_out_mat->f[ 0] =  f_cos; p_out_mat->f[ 4] = f_sin; p_out_mat->f[ 8] = 0.0f; p_out_mat->f[12] = 0.0f;
	p_out_mat->f[ 1] = -f_sin; p_out_mat->f[ 5] = f_cos; p_out_mat->f[ 9] = 0.0f; p_out_mat->f[13] = 0.0f;
	p_out_mat->f[ 2] =  0.0f;  p_out_mat->f[ 6] = 0.0f;  p_out_mat->f[10] = 1.0f; p_out_mat->f[14] = 0.0f;
	p_out_mat->f[ 3] =  0.0f;  p_out_mat->f[ 7] = 0.0f;  p_out_mat->f[11] = 0.0f; p_out_mat->f[15] = 1.0f;
}

void D3DXVec3Normalize(TD3DVector3* p_out_mat, const TD3DVector3* p_vec)
{
	double distance = (double)(p_vec->x * p_vec->x + p_vec->y * p_vec->y + p_vec->z * p_vec->z);
	distance = 1.0 / sqrt(distance);

	float f_distance = (float)distance;

	p_out_mat->x = p_vec->x * f_distance;
	p_out_mat->y = p_vec->y * f_distance;
	p_out_mat->z = p_vec->z * f_distance;
}

void D3DXVec3CrossProduct(TD3DVector3* p_out_mat, const TD3DVector3* p_vec1, const TD3DVector3* p_vec2)
{
	TD3DVector3 out_vec;

	out_vec.x = p_vec1->y * p_vec2->z - p_vec1->z * p_vec2->y;
	out_vec.y = p_vec1->z * p_vec2->x - p_vec1->x * p_vec2->z;
	out_vec.z = p_vec1->x * p_vec2->y - p_vec1->y * p_vec2->x;

	*p_out_mat = out_vec;
}

void D3DXMatrixLookAtLH(TD3DMatrix* p_out_mat, const TD3DVector3* p_eye, const TD3DVector3* p_at, const TD3DVector3* p_up)
{
	TD3DVector3 f_vec(p_eye->x - p_at->x, p_eye->y - p_at->y, p_eye->z - p_at->z);
	TD3DVector3 s_vec;
	TD3DVector3 u_vec;

	D3DXVec3Normalize(&f_vec, &f_vec);

	{
		TD3DVector3 up_normalized;

		D3DXVec3Normalize(&up_normalized, p_up);

		D3DXVec3CrossProduct(&s_vec, &f_vec, &up_normalized);
	}

	D3DXVec3CrossProduct(&u_vec, &s_vec, &f_vec);

	p_out_mat->f[ 0] = s_vec.x;
	p_out_mat->f[ 1] = u_vec.x;
	p_out_mat->f[ 2] = - f_vec.x;
	p_out_mat->f[ 3] = 0.0f;

	p_out_mat->f[ 4] = s_vec.y;
	p_out_mat->f[ 5] = u_vec.y;
	p_out_mat->f[ 6] = - f_vec.y;
	p_out_mat->f[ 7] = 0.0f;

	p_out_mat->f[ 8] = s_vec.z;
	p_out_mat->f[ 9] = u_vec.z;
	p_out_mat->f[10] = - f_vec.z;
	p_out_mat->f[11] = 0.0f;

	p_out_mat->f[12] = 0.0f;
	p_out_mat->f[13] = 0.0f;
	p_out_mat->f[14] = 0.0f;
	p_out_mat->f[15] = 1.0f;

	TD3DMatrix temp_mat;
	D3DXMatrixTranslation(&temp_mat, -p_eye->x, -p_eye->y, -p_eye->z);

	D3DXMatrixMultiply(p_out_mat, &temp_mat, p_out_mat);
}

void D3DXMatrixLookAtRH(TD3DMatrix* p_out_mat, const TD3DVector3* p_eye, const TD3DVector3* p_at, const TD3DVector3* p_up)
{
	TD3DVector3 f_vec(p_at->x - p_eye->x, p_at->y - p_eye->y, p_at->z - p_eye->z);
	TD3DVector3 s_vec;
	TD3DVector3 u_vec;

	D3DXVec3Normalize(&f_vec, &f_vec);

	{
		TD3DVector3 up_normalized;

		D3DXVec3Normalize(&up_normalized, p_up);

		D3DXVec3CrossProduct(&s_vec, &f_vec, &up_normalized);
	}

	D3DXVec3CrossProduct(&u_vec, &s_vec, &f_vec);

	p_out_mat->f[ 0] = s_vec.x;
	p_out_mat->f[ 1] = u_vec.x;
	p_out_mat->f[ 2] = - f_vec.x;
	p_out_mat->f[ 3] = 0.0f;

	p_out_mat->f[ 4] = s_vec.y;
	p_out_mat->f[ 5] = u_vec.y;
	p_out_mat->f[ 6] = - f_vec.y;
	p_out_mat->f[ 7] = 0.0f;

	p_out_mat->f[ 8] = s_vec.z;
	p_out_mat->f[ 9] = u_vec.z;
	p_out_mat->f[10] = - f_vec.z;
	p_out_mat->f[11] = 0.0f;

	p_out_mat->f[12] = 0.0f;
	p_out_mat->f[13] = 0.0f;
	p_out_mat->f[14] = 0.0f;
	p_out_mat->f[15] = 1.0f;

	TD3DMatrix temp_mat;
	D3DXMatrixTranslation(&temp_mat, -p_eye->x, -p_eye->y, -p_eye->z);

	D3DXMatrixMultiply(p_out_mat, &temp_mat, p_out_mat);
}

void D3DXMatrixPerspectiveFovLH(TD3DMatrix* p_out_mat, float fovy, float aspect_ratio, float z_near, float z_far)
{
	float f, n, real_aspect;
	
	if (IS_ROTATED)
		real_aspect = 1.0f / aspect_ratio;
	else
		real_aspect = aspect_ratio;

	// cotangent(a) == 1.0f / tan(a);
	f = 1.0f / (float)tan(fovy * 0.5f);
	n = 1.0f / (z_far - z_near);

	p_out_mat->f[ 0] = f / real_aspect;
	p_out_mat->f[ 1] = 0;
	p_out_mat->f[ 2] = 0;
	p_out_mat->f[ 3] = 0;

	p_out_mat->f[ 4] = 0;
	p_out_mat->f[ 5] = f;
	p_out_mat->f[ 6] = 0;
	p_out_mat->f[ 7] = 0;

	p_out_mat->f[ 8] = 0;
	p_out_mat->f[ 9] = 0;
	p_out_mat->f[10] = z_far * n;
	p_out_mat->f[11] = 1;

	p_out_mat->f[12] = 0;
	p_out_mat->f[13] = 0;
	p_out_mat->f[14] = -z_far * z_near * n;
	p_out_mat->f[15] = 0;

	if (IS_ROTATED)
	{
		TD3DMatrix mat_rotation;
		TD3DMatrix mat_temp = *p_out_mat;

		D3DXMatrixRotationZ(&mat_rotation, 90.0f * D3DX_PI / 180.0f);
		D3DXMatrixMultiply(p_out_mat, &mat_temp, &mat_rotation);
	}
}

void D3DXMatrixPerspectiveFovRH(TD3DMatrix* p_out_mat, float fovy, float aspect_ratio, float z_near, float z_far)
{
	float f, n, real_aspect;
	
	if (IS_ROTATED)
		real_aspect = 1.0f / aspect_ratio;
	else
		real_aspect = aspect_ratio;

	// cotangent(a) == 1.0f / tan(a);
	f = 1.0f / (float)tan(fovy * 0.5f);
	n = 1.0f / (z_near - z_far);

	p_out_mat->f[ 0] = f / real_aspect;
	p_out_mat->f[ 1] = 0;
	p_out_mat->f[ 2] = 0;
	p_out_mat->f[ 3] = 0;

	p_out_mat->f[ 4] = 0;
	p_out_mat->f[ 5] = f;
	p_out_mat->f[ 6] = 0;
	p_out_mat->f[ 7] = 0;

	p_out_mat->f[ 8] = 0;
	p_out_mat->f[ 9] = 0;
	p_out_mat->f[10] = (z_far + z_near) * n;
	p_out_mat->f[11] = -1;

	p_out_mat->f[12] = 0;
	p_out_mat->f[13] = 0;
	p_out_mat->f[14] = (2 * z_far * z_near) * n;
	p_out_mat->f[15] = 0;

	if (IS_ROTATED)
	{
		TD3DMatrix mat_rotation;
		TD3DMatrix mat_temp = *p_out_mat;

		D3DXMatrixRotationZ(&mat_rotation, -90.0f * D3DX_PI / 180.0f);
		D3DXMatrixMultiply(p_out_mat, &mat_temp, &mat_rotation);
	}
}

void D3DXMatrixOrthoRHF(TD3DMatrix* p_out_mat, float width, float height, float z_near, float z_far)
{
	p_out_mat->f[ 0] = 2 / width;
	p_out_mat->f[ 1] = 0;
	p_out_mat->f[ 2] = 0;
	p_out_mat->f[ 3] = 0;

	p_out_mat->f[ 4] = 0;
	p_out_mat->f[ 5] = 2 / height;
	p_out_mat->f[ 6] = 0;
	p_out_mat->f[ 7] = 0;

	p_out_mat->f[ 8] = 0;
	p_out_mat->f[ 9] = 0;
	p_out_mat->f[10] = 1 / (z_near - z_far);
	p_out_mat->f[11] = z_near / (z_near - z_far);

	p_out_mat->f[12] = 0;
	p_out_mat->f[13] = 0;
	p_out_mat->f[14] = 0;
	p_out_mat->f[15] = 1;

	if (IS_ROTATED)
	{
		TD3DMatrix mat_rotation;
		TD3DMatrix mat_temp = *p_out_mat;

		D3DXMatrixRotationZ(&mat_rotation, -90.0f * D3DX_PI / 180.0f);
		D3DXMatrixMultiply(p_out_mat, &mat_rotation, &mat_temp);
	}
}

void D3DXMatrixOrthoOffCenterLH(TD3DMatrix* p_out_mat, float left, float right, float bottom, float top, float z_near, float z_far)
{
	D3DXMatrixOrthoRHF(p_out_mat, right - left, bottom - top, z_near, z_far);
}

void D3DXDeclaratorFromFVF(DWORD fvf, D3DVERTEXELEMENT9* p_out_declaration)
{
}
