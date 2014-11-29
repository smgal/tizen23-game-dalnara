
#ifndef __IQB_CLASS_3D_VERTEX_H__
#define __IQB_CLASS_3D_VERTEX_H__

#include "iqb_class_3d.h"
#include "iqb_class_3d_texture.h"
#include "iqb_class_3d_shader.h"

namespace erio
{
	enum EVertexFormat
	{
		VERTEXFORMAT_POS_NOR_TEX,
		VERTEXFORMAT_POS_DIF_TEX,
		VERTEXFORMAT_POS_TEX,
		VERTEXFORMAT_POS_DIF,
		VERTEXFORMAT_POS,
		VERTEXFORMAT_POS_NOR,
	};

	template <EVertexFormat vertex_format>
	struct TVertexDataTraits
	{
	};

	template <>
	struct TVertexDataTraits<VERTEXFORMAT_POS_NOR_TEX>
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
		};

		struct TVertexData
		{
			TD3DVector3 position;
			TD3DVector3 normal;
			TD3DVector2 tex_pos;
		};
	};

	template <>
	struct TVertexDataTraits<VERTEXFORMAT_POS_DIF_TEX>
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
		};

		struct TVertexData
		{
			TD3DVector3 position;
			TD3DVector4 color;
			TD3DVector2 tex_pos;
		};
	};

	template <>
	struct TVertexDataTraits<VERTEXFORMAT_POS_TEX>
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_TEX1,
		};

		struct TVertexData
		{
			TD3DVector3 position;
			TD3DVector2 tex_pos;
		};
	};

	template <>
	struct TVertexDataTraits<VERTEXFORMAT_POS_DIF>
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE,
		};

		struct TVertexData
		{
			TD3DVector3 position;
			TD3DVector4 color;
		};
	};

	template <>
	struct TVertexDataTraits<VERTEXFORMAT_POS>
	{
		enum
		{
			FVF = D3DFVF_XYZ,
		};
	
		struct TVertexData
		{
			TD3DVector3 position;
		};
	};

	template <>
	struct TVertexDataTraits<VERTEXFORMAT_POS_NOR>
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_NORMAL,
		};

		struct TVertexData
		{
			TD3DVector3 position;
			TD3DVector3 normal;
		};
	};

	template <EVertexFormat vertex_format>
	struct TVertexBuffer
	{
	public:
		typedef typename TVertexDataTraits<vertex_format>::TVertexData TVertexData;

		enum
		{
			FVF = TVertexDataTraits<vertex_format>::FVF
		};

		IDirect3DVertexBuffer9*  p_vertex_buffer;
		iu::shared_ptr<CTexture> texture;

	protected:
		IDirect3DDevice9* m_p_d3d_device;
		TShaderSet        m_shader_set;
		unsigned int      m_size_vertex;
		unsigned int      m_count_vertex;

	public:
		TVertexBuffer(IDirect3DDevice9* p_d3d_device, TVertexData* p_data, unsigned int size, unsigned int count, iu::shared_ptr<CTexture> _texture = iu::shared_ptr<CTexture>((CTexture*)NULL))
			: p_vertex_buffer(NULL)
			, texture(_texture)
			, m_p_d3d_device(p_d3d_device)
			, m_size_vertex(size)
			, m_count_vertex(count)
		{
			if (_SUCCEEDED(p_d3d_device->CreateVertexBuffer(m_size_vertex*m_count_vertex*sizeof(TVertexData), D3DUSAGE_WRITEONLY, FVF, D3DPOOL_MANAGED, &p_vertex_buffer, 0)))
			{
				Assign(p_data, 0, m_size_vertex*m_count_vertex);
			}
		}

		virtual ~TVertexBuffer()
		{
			if (p_vertex_buffer)
			{
				p_vertex_buffer->Release();
				p_vertex_buffer = NULL;
			}
		}

		void SetShader(TShaderSet shader_set)
		{
			m_shader_set = shader_set;
		}

		void DrawPrimitive(D3DPRIMITIVETYPE type, unsigned int ix_start, unsigned int count)
		{
			m_p_d3d_device->DrawPrimitive(type, ix_start, count);
		}

		void Assign(TVertexData* p_data, unsigned int index, unsigned int num)
		{
			if (p_vertex_buffer)
			{
				//?? check if range is valid
				void* p_dest = NULL;

				if (_SUCCEEDED(p_vertex_buffer->Lock(index, num, &p_dest, 0)))
				{
					memcpy(p_dest, p_data, num*sizeof(TVertexData));
					p_vertex_buffer->Unlock();
				}
			}
		}

		virtual void Begin(void)
		{
			if (texture.get())
				m_p_d3d_device->SetTexture(0, texture->m_p_texture);
			else
				m_p_d3d_device->SetTexture(0, NULL);

			m_p_d3d_device->SetStreamSource(0, p_vertex_buffer, 0, sizeof(TVertexData));

			if (m_shader_set.vertex.get())
			{
				m_p_d3d_device->SetVertexDeclaration(m_shader_set.vertex->p_declation);
				m_p_d3d_device->SetVertexShader(m_shader_set.vertex->p_shader);
			}
			else
			{
				m_p_d3d_device->SetFVF(FVF);
			}

			if (m_shader_set.pixel.get())
			{
				m_p_d3d_device->SetPixelShader(m_shader_set.pixel->p_shader);
			}

			{
				IDirect3DDevice9::TStatus::TStreamSource stream_source = m_p_d3d_device->m_status.stream_source[0];

				float* p_stream_data = (float*)stream_source.p_stream_data->m_p_buffer;
				int    stream_data_offset = 0;
				DWORD  fvf = stream_source.p_stream_data->m_fvf;

				if (fvf & D3DFVF_XYZ)
				{
					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(3, GL_FLOAT, stream_source.stride, p_stream_data + stream_data_offset);

					stream_data_offset += 3;
				}
				else
				{
					glDisableClientState(GL_VERTEX_ARRAY);
				}

				if (fvf & D3DFVF_DIFFUSE)
				{
					glEnableClientState(GL_COLOR_ARRAY);
					glColorPointer(4, GL_FLOAT, stream_source.stride, p_stream_data + stream_data_offset);

					stream_data_offset += 4;
				}
				else
				{
					glDisableClientState(GL_COLOR_ARRAY);
				}

				if (fvf & D3DFVF_NORMAL)
				{
					glEnableClientState(GL_NORMAL_ARRAY);
					glNormalPointer(GL_FLOAT, stream_source.stride, p_stream_data + stream_data_offset);
					glEnable(GL_NORMALIZE);

					glEnable(GL_LIGHTING);

					stream_data_offset += 3;
				}
				else
				{
					glDisable(GL_LIGHTING);
					glDisableClientState(GL_NORMAL_ARRAY);
				}

				if (fvf & D3DFVF_TEX1)
				{
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(2, GL_FLOAT, stream_source.stride, p_stream_data + stream_data_offset);

					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glEnable(GL_ALPHA_TEST);
					glAlphaFunc(GL_GREATER, 0);

					if (texture.get())
					{
						GLuint h_texture = ((avej_lite::TSurfaceDesc*)texture->m_p_texture->m_p_surface->Handle())->hw_handle;

						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, h_texture);
						glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					}

					stream_data_offset += 2;
				}
				else
				{
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
			}
		}

		virtual void End(void)
		{
			m_p_d3d_device->SetVertexShader(NULL);
			m_p_d3d_device->SetPixelShader(NULL);
		}
	};

	template <EVertexFormat vertex_format>
	struct TVertexIndexBuffer: public TVertexBuffer<vertex_format>
	{
		typedef typename TVertexBuffer<vertex_format>::TVertexData TVertexData;

	public:
		IDirect3DIndexBuffer9* p_index_buffer;
		//iu::vector<iu::pair<int, int> > ready_buffer;
		iu::vector<short> vertex_buffer;

		TVertexIndexBuffer(IDirect3DDevice9* p_d3d_device, TVertexData* p_vertex_data, unsigned int size, unsigned int count, short* p_index_data, unsigned int num_index, iu::shared_ptr<CTexture> _texture)
			: TVertexBuffer<vertex_format>(p_d3d_device, p_vertex_data, size, count, _texture)
			, p_index_buffer(NULL)
		{
			if (_SUCCEEDED(p_d3d_device->CreateIndexBuffer(num_index*sizeof(short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &p_index_buffer, 0)))
			{
				void* p_dest = NULL;

				if (_SUCCEEDED(p_index_buffer->Lock(0, 0, &p_dest, 0)))
				{
					memcpy(p_dest, p_index_data, num_index*sizeof(short));
					p_index_buffer->Unlock();
				}
			}
		}

		~TVertexIndexBuffer()
		{
			if (p_index_buffer)
			{
				p_index_buffer->Release();
				p_index_buffer = NULL;
			}
		}
		
		void DrawIndexedPrimitive(D3DPRIMITIVETYPE type, unsigned int ix_start, unsigned int count)
		{
	#if 1
			this->m_p_d3d_device->DrawIndexedPrimitive(type, ix_start, 0, this->m_size_vertex * this->m_count_vertex, 0, count);
	#else
			{
				short* p_src = (short*)this->m_p_d3d_device->m_status.p_index_buffer->m_p_buffer;

				for (int i = 0; i < count * 3; i++)
					vertex_buffer.push_back(*p_src++ + ix_start);

				int size = vertex_buffer.size();

				if (size > 2048)
				{
					glDrawElements(GL_TRIANGLES, vertex_buffer.size(), GL_UNSIGNED_SHORT, vertex_buffer.begin());
					vertex_buffer.clear();
				}
			}
	#endif
		}

		virtual void Begin(void)
		{
			TVertexBuffer<vertex_format>::Begin();

			this->m_p_d3d_device->SetIndices(p_index_buffer);

			vertex_buffer.clear();
		}
		
		virtual void End(void)
		{
			TVertexBuffer<vertex_format>::End();
		}

		virtual void Flush(void)
		{
	#if 0
			glDrawElements(GL_TRIANGLES, vertex_buffer.size(), GL_UNSIGNED_SHORT, vertex_buffer.begin());
	#endif
		}
	};

} // namespace erio

#endif // #ifndef __IQB_CLASS_3D_VERTEX_H__
