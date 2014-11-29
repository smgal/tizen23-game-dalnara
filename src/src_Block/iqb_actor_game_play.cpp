
////////////////////////////////////////////////////////////////////////////////
// uses

#include "iqb_actor.h"
#include "iqb_base_gfx.h"
#include "iqb_class_3d_vertext.h"
#include "iqb_class_3d_texture.h"
#include "iqb_class_3d_attrib.h"
#include "iqb_data_map.h"

#include <map>
#include <math.h>

using namespace avej_lite;

namespace erio
{
	extern IDirect3DDevice9* g_p_d3d_device;
}

// temporary position
namespace
{
	int STAGE = 4;

	struct TOldMapData
	{
		enum { NUM_STAGE = 15 };
		enum { NAME_STRING_SIZE = 22 };
		enum { MAP_SIZE = 17 };
		struct
		{
			char* sz_name[NAME_STRING_SIZE];
			unsigned char map[MAP_SIZE][MAP_SIZE];
		} map_data[NUM_STAGE];
	};

	class COldMapData: public TOldMapData
	{
	public:
		enum TFixedObject
		{
			FIXEDOBJECT_NORMAL,
			FIXEDOBJECT_MINE,
			FIXEDOBJECT_WATER,
			FIXEDOBJECT_RECLAIM,
			FIXEDOBJECT_WALL,
			FIXEDOBJECT_DECORATION
		};

		enum TFieldObject
		{
			FIELDOBJECT_NONE,
			FIELDOBJECT_BLOCK,
			FIELDOBJECT_GOAL_BLOCK,
			FIELDOBJECT_GOAL_PC,
			FIELDOBJECT_PC
		};

		struct TObjectSet
		{
			TFixedObject fixed_object;
			TFieldObject field_object;

			TObjectSet(TFixedObject _fixed_object, TFieldObject _field_object)
				: fixed_object(_fixed_object)
				, field_object(_field_object)
			{
			}
		};

		COldMapData(const util::string& file_name)
		{
			util::CFileReadStream read_stream(file_name);

			// 4665
			// = NUM_STAGE * 311
			// = NUM_STAGE * (NAME_STRING_SIZE + 289)
			// = NUM_STAGE * (NAME_STRING_SIZE + (MAP_SIZE * MAP_SIZE))
			const int FILE_SIZE = NUM_STAGE * (NAME_STRING_SIZE + (MAP_SIZE * MAP_SIZE));

			if (read_stream.IsValid() && read_stream.GetSize() == FILE_SIZE)
			{
				for (int stage = 0; stage < NUM_STAGE; stage++)
				{
					read_stream.Read(&map_data[stage].sz_name[0], NAME_STRING_SIZE);
					read_stream.Read(&map_data[stage].map[0][0], MAP_SIZE * MAP_SIZE);
				}
			}
		}

		void DisplayStage(int stage)
		{
			unsigned long COLOR_DATA[] =
			{
				0xFF000000, // normal
				0xFFFF00FF, // movable block
				0xFF800000, // movable block + mine
				0xFFFF0000, // mine
				0xFF0000FF, // water
				0xFF00FF00, // goal by block
				0xFF00FFFF, // goal by person
				0xFFFFFFFF, // fixed wall
				0xFFFFFF00, // start point
				0x80808080  // ....
			};

			const int TILE_DISPLAY_SIZE = 8;

			for (int y = 0; y < MAP_SIZE; y++)
			for (int x = 0; x < MAP_SIZE; x++)
			{
				int ix_map = map_data[stage].map[x][y];
				ix_map = (ix_map > 9) ? 9 : ix_map;

				erio::gfx::FillRect(COLOR_DATA[ix_map], 30+TILE_DISPLAY_SIZE*x, 30+TILE_DISPLAY_SIZE*y, TILE_DISPLAY_SIZE, TILE_DISPLAY_SIZE); 
			}
		}

		TObjectSet GetMapData(int stage, int x, int y)
		{
			if (stage < 0 || stage >= NUM_STAGE)
				return TObjectSet(FIXEDOBJECT_NORMAL, FIELDOBJECT_NONE);

			if ((x < 0 || x >= MAP_SIZE) || (y < 0 || y >= MAP_SIZE))
				return TObjectSet(FIXEDOBJECT_NORMAL, FIELDOBJECT_NONE);

			switch (map_data[stage].map[x][y])
			{
			case 0: // normal
				return TObjectSet(FIXEDOBJECT_NORMAL, FIELDOBJECT_NONE);
			case 1: // movable block
				return TObjectSet(FIXEDOBJECT_NORMAL, FIELDOBJECT_BLOCK);
			case 2: // movable block + mine
				return TObjectSet(FIXEDOBJECT_MINE, FIELDOBJECT_BLOCK);
			case 3: // mine
				return TObjectSet(FIXEDOBJECT_MINE, FIELDOBJECT_NONE);
			case 4: // water
				return TObjectSet(FIXEDOBJECT_WATER, FIELDOBJECT_NONE);
			case 5: // goal by block
				return TObjectSet(FIXEDOBJECT_NORMAL, FIELDOBJECT_GOAL_BLOCK);
			case 6: // goal by person
				return TObjectSet(FIXEDOBJECT_NORMAL, FIELDOBJECT_GOAL_PC);
			case 7: // fixed wall
				return TObjectSet(FIXEDOBJECT_WALL, FIELDOBJECT_NONE);
			case 8: // start point
				return TObjectSet(FIXEDOBJECT_NORMAL, FIELDOBJECT_PC);
			default:
				return TObjectSet(FIXEDOBJECT_DECORATION, FIELDOBJECT_NONE);
			}
		}

	};
}

namespace
{
	using namespace erio;

	struct TBounds
	{
		float x1;
		float y1;
		float x2;
		float y2;

		TBounds()
		{
		}

		TBounds(float _x1, float _y1, float _x2, float _y2)
			: x1(_x1)
			, y1(_y1)
			, x2(_x2)
			, y2(_y2)
		{
		}
	};

	void s_ConvertIsometricToVertice(TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>::TVertexData vertices[], unsigned int count, TBounds src_rect, IDirect3DTexture9* p_texture)
	{
		const float MAGIC_NUMBER = 23.0f;
		const float read_width  = sqrtf(powf(MAGIC_NUMBER,2.0f)+powf(src_rect.x2-src_rect.x1,2.0f)) / 2.0f;
		const float TALL = (src_rect.y2 - src_rect.y1 - MAGIC_NUMBER) / read_width;

		// Vertex position ���� �����
		{
			vertices[ 4].position = D3DVECTOR3(-0.5f, 0.0f, 0.5f);
			vertices[ 5].position = D3DVECTOR3(-0.5f, TALL, 0.5f);
			vertices[ 6].position = D3DVECTOR3(-0.5f, 0.0f,-0.5f);
			vertices[ 7].position = D3DVECTOR3(-0.5f, TALL,-0.5f);
			vertices[ 8].position = D3DVECTOR3( 0.5f, 0.0f,-0.5f);
			vertices[ 9].position = D3DVECTOR3( 0.5f, TALL,-0.5f);
			vertices[10].position = D3DVECTOR3( 0.5f, 0.0f, 0.5f);
			vertices[11].position = D3DVECTOR3( 0.5f, TALL, 0.5f);
			vertices[12].position = vertices[ 4].position;
			vertices[13].position = vertices[ 5].position;
			//
			vertices[ 0].position = vertices[ 5].position;
			vertices[ 1].position = vertices[ 7].position;
			vertices[ 2].position = vertices[ 9].position;
			vertices[ 3].position = vertices[11].position;
		}

		// Vertex normal ���� �����
		{
			for (int i = 0; i < count; i++)
				D3DXVec3Normalize(&vertices[i].normal, &vertices[i].position);
		}

		// Vertex UV ���� �����
		{
			D3DSURFACE_DESC desc;
			p_texture->GetLevelDesc(0, &desc);

			float w_texture = float(desc.Width);
			float h_texture = float(desc.Height);

			float src_rect_x0 = (src_rect.x1 + src_rect.x2) / 2.0f;

			/*
				(x1,y2) -> (x1,y1) -> (x2,y2) -> (x2,y1) ��
			*/
			vertices[ 4].tex_pos = TD3DVector2(src_rect.x1 + 0.5f, src_rect.y2 - MAGIC_NUMBER / 2.0f);
			vertices[ 5].tex_pos = TD3DVector2(src_rect.x1 + 0.5f, src_rect.y1 + MAGIC_NUMBER / 2.0f);
			vertices[ 6].tex_pos = TD3DVector2(src_rect_x0,        src_rect.y2 + 0.5f);
			vertices[ 7].tex_pos = TD3DVector2(src_rect_x0,        src_rect.y1 + MAGIC_NUMBER + 0.5f);
			vertices[ 8].tex_pos = TD3DVector2(src_rect.x2 - 0.5f, src_rect.y2 - MAGIC_NUMBER / 2.0f);
			vertices[ 9].tex_pos = TD3DVector2(src_rect.x2 - 0.5f, src_rect.y1 + MAGIC_NUMBER / 2.0f);
			// �ݴ��� ���
			vertices[10].tex_pos = vertices[ 6].tex_pos;
			vertices[11].tex_pos = vertices[ 7].tex_pos;
			vertices[12].tex_pos = vertices[ 4].tex_pos;
			vertices[13].tex_pos = vertices[ 5].tex_pos;
			// �����
			vertices[ 0].tex_pos = vertices[ 5].tex_pos;
			vertices[ 1].tex_pos = vertices[ 7].tex_pos;
			vertices[ 2].tex_pos = TD3DVector2(src_rect.x2 - 0.5f, src_rect.y1 + MAGIC_NUMBER / 2.0f);
			vertices[ 3].tex_pos = TD3DVector2(src_rect_x0, src_rect.y1);

			for (int i = 0; i < count; i++)
			{
				vertices[i].tex_pos.x /= w_texture;
				vertices[i].tex_pos.y /= h_texture;
			}
		}
	}

	void s_SetUpGl(void)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_COLOR_MATERIAL);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_ALPHA_TEST);
		glAlphaFuncx(GL_GREATER, GLclampx(0));
	}

	void s_SetupMatrices(void)
	{
		TD3DMatrix mat_world;

		D3DXMatrixIdentity(&mat_world);
		g_p_d3d_device->SetTransform(D3DTS_WORLD, &mat_world);
	}

	class CRenderMode
	{
	public:
		enum ERenderMode
		{
			SPRITE = 0,
			GHOST
		};

		CRenderMode(ERenderMode mode)
		{
			DWORD factor = 0xFF;

			switch (mode)
			{
			case SPRITE:
				factor = 0xD0;
				g_p_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
				break;
			case GHOST:
				factor = 0x20;
				g_p_d3d_device->SetRenderState(D3DRS_ZENABLE, false);
				g_p_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
				break;
			};
			g_p_d3d_device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(factor, 0xFF, 0xFF, 0xFF));
			g_p_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

			g_p_d3d_device->SetRenderState(D3DRS_LIGHTING, 0);

			shader::SetTexFactor(g_p_d3d_device, float(factor) / 255.0f);
		};

		~CRenderMode()
		{
			shader::SetTexFactor(g_p_d3d_device, 1.0f);

			g_p_d3d_device->SetRenderState(D3DRS_LIGHTING, 1);

			g_p_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);

			g_p_d3d_device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
			g_p_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			g_p_d3d_device->SetRenderState(D3DRS_ZENABLE, true);
		};
	};

	const char* MAP_DATA[] =
	{
		"00000000000",
		"04111100300",
		"01000000330",
		"01022220300",
		"01000020000",
		"01000020000",
		"00000022222",
		"50000000200",
		"00300000200",
		"03333300000",
		"00300000000",
		NULL,
	};

	typedef iu::vector<CSmPlayer*> TPlayerList;

	struct TResource
	{
		TResource()
			: p_d3d_attrib(0)
			, p_vb_character(0)
			, p_vb_tile(0)
			, p_vb_temp(0)
			, p_vb_fixed_map(0)
			, map(MAP_DATA, -5, -5)
		{
		}
		
		~TResource()
		{
			for (TPlayerList::iterator obj = player_list.begin(); obj < player_list.end(); ++obj)
			{
				delete *obj;
			}

			delete p_vb_character;
			delete p_vb_tile;
			delete p_vb_fixed_map;
			delete p_vb_temp;

			delete p_d3d_attrib;
		}

		CSm3DAttrib* p_d3d_attrib;

		TVertexBuffer<VERTEXFORMAT_POS_TEX>* p_vb_character;
		TVertexIndexBuffer<VERTEXFORMAT_POS_NOR_TEX>* p_vb_tile;
		TVertexBuffer<VERTEXFORMAT_POS_DIF>* p_vb_temp;
		TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>* p_vb_fixed_map;

		CTileMap map;
		TPlayerList player_list;
		iu::shared_ptr<CTexture> sprite;
		std::map<EVertexFormat, TShaderSet> shader_set;
	};

	const int NUM_OF_VERTICES = 14;

	TResource* p_resource = 0;
	COldMapData* p_old_map_data = 0;

	CSmPlayer* GetMainPlayer(void)
	{
		return (p_resource->player_list.empty()) ? NULL : p_resource->player_list[0];
	}

	void DisplayMainCharacter(int mode)
	{
		const int MAX_FACE_INC = 8;

		CRenderMode state((CRenderMode::ERenderMode)mode);
		
		CSmPlayer* p_main_player = GetMainPlayer();

		{
			single angle = p_resource->p_d3d_attrib->GetCamera()->GetAngle();

			p_resource->p_vb_character->Begin();

			TD3DMatrix mat_view;
			TD3DMatrix mat_proj;
			TD3DMatrix mat_wv;
			TD3DMatrix mat_wvp;
			TD3DMatrix mat_vp;

			g_p_d3d_device->GetTransform(D3DTS_VIEW, &mat_view);
			g_p_d3d_device->GetTransform(D3DTS_PROJECTION, &mat_proj);

			D3DXMatrixMultiply(&mat_vp, &mat_view, &mat_proj);
			D3DXMatrixTranspose(&mat_vp, &mat_vp);

			g_p_d3d_device->SetVertexShaderConstantF(16, (float*)&mat_vp, 4);

			for (TPlayerList::iterator obj = p_resource->player_list.begin(); obj < p_resource->player_list.end(); ++obj)
			{
				{
					TD3DMatrix mat_world;
					TD3DMatrix temp;

					D3DXMatrixIdentity(&mat_world);
					D3DXMatrixScaling(&mat_world, 1.0f, 1.8f, 1.0f);
					D3DXMatrixRotationY(&temp, angle);
					D3DXMatrixMultiply(&mat_world, &mat_world, &temp);
					D3DXMatrixTranslation(&temp, 1.0f * (*obj)->attribute.pos.x - p_main_player->attribute.pos.x, 0.26f, 1.0f * (*obj)->attribute.pos.y - p_main_player->attribute.pos.y);
					D3DXMatrixMultiply(&mat_world, &mat_world, &temp);

					g_p_d3d_device->SetTransform(D3DTS_WORLD, &mat_world);

					{
						D3DXMatrixMultiply(&mat_wv, &mat_world, &mat_view);
						D3DXMatrixMultiply(&mat_wvp, &mat_wv, &mat_proj);

						D3DXMatrixTranspose(&mat_wvp, &mat_wvp);
						g_p_d3d_device->SetVertexShaderConstantF(4, (float*)&mat_wvp, 4);

						D3DXMatrixTranspose(&mat_wv, &mat_wv);
						g_p_d3d_device->SetVertexShaderConstantF(8, (float*)&mat_wv, 4);

						D3DXMatrixTranspose(&mat_world, &mat_world);
						g_p_d3d_device->SetVertexShaderConstantF(12, (float*)&mat_world, 4);
					}
				}

				/*
				�����带 ���� ����� �̸� ���Ǿ singleton�� �Ǿ��...

				ĳ���Ϳ��� vertex buffer�� �� buffer�� �ؼ��ϴ�(character�� ���� ��Ŀ� �°�) �����
				���� ��޵Ǿ�� �Ѵ� �׷��� �Ʒ��� ���� ���ĵ� �Ϲ�ȭ�� �� �־�� �Ѵ�.

				�� vertex buffer�� ���° �ε��� ��������,
				8���⿡ ���� frame�� ������ �� �־�� �ϰ�, frame�� ������ repeat���� reverse���� ���������� �����Ǿ�� �Ѵ�.
				*/

				int ixFace = (*obj)->attribute.face * 2 + (*obj)->attribute.face_inc;
				assert(ixFace >= 0 && ixFace < MAX_FACE_INC);

				p_resource->p_vb_character->DrawPrimitive(D3DPT_TRIANGLESTRIP, ixFace*4, 2);
			}

			p_resource->p_vb_character->End();
		}
	}

	void ProcessObjects(const CInputDevice& input_device)
	{
		const float MOVE = 0.075f;
		float dx = 0.0f; 
		float dy = 0.0f; 

		if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_LEFT))
			dx = -MOVE;
		if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_RIGHT))
			dx = +MOVE;
		if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_UP))
			dy = +MOVE;
		if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_DOWN))
			dy = -MOVE;

		if ((dx != 0.0f) || (dy != 0.0f))
		{
			const float RADIUS = 0.4f;

			float x_center  = GetMainPlayer()->attribute.pos.x + dx + 0.5f;
			float y_center  = GetMainPlayer()->attribute.pos.y + dy + 0.5f;

			bool  walkable = p_resource->map.IsMovableMapRect(x_center, y_center, RADIUS);

			if (walkable)
			{
				GetMainPlayer()->Move(dx, dy);
			}
			else
			{
				walkable = p_resource->map.IsMovableMapRect(x_center, GetMainPlayer()->attribute.pos.y + 0.5f, RADIUS);
				if (walkable && (dx != 0.0f))
				{
					GetMainPlayer()->Move(dx, 0.0f);
				}
				else
				{
					walkable = p_resource->map.IsMovableMapRect(GetMainPlayer()->attribute.pos.x + 0.5f, y_center, RADIUS);
					if (walkable && (dy != 0.0f))
						GetMainPlayer()->Move(0.0f, dy);
				}
			}
		}
	}

	void DisplayObjects(void)
	{
		s_SetUpGl();
		p_resource->p_d3d_attrib->Process();

		g_p_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

		if (_SUCCEEDED(g_p_d3d_device->BeginScene()))
		{
			s_SetupMatrices();

			{
				CSmPlayer* p_main_player = GetMainPlayer();

				{
					const int MAP_RANGE = 14;

					TD3DMatrix mat_view;
					TD3DMatrix mat_proj;
					TD3DMatrix mat_wv;
					TD3DMatrix mat_wvp;
					TD3DMatrix mat_vp;

					g_p_d3d_device->GetTransform(D3DTS_VIEW, &mat_view);
					g_p_d3d_device->GetTransform(D3DTS_PROJECTION, &mat_proj);

					D3DXMatrixMultiply(&mat_vp, &mat_view, &mat_proj);
					D3DXMatrixTranspose(&mat_vp, &mat_vp);

					g_p_d3d_device->SetVertexShaderConstantF(16, (float*)&mat_vp, 4);
#if 1
					p_resource->p_vb_fixed_map->Begin();
					{
						// float x_offset = p_main_player->attribute.pos.x - floorf(p_main_player->attribute.pos.x);
						// float y_offset = p_main_player->attribute.pos.y - floorf(p_main_player->attribute.pos.y);

						TD3DMatrix mat_world;
						D3DXMatrixIdentity(&mat_world);
						D3DXMatrixTranslation(&mat_world, - p_main_player->attribute.pos.x, 0.0f, - p_main_player->attribute.pos.y);
						g_p_d3d_device->SetTransform(D3DTS_WORLD, &mat_world);

						p_resource->p_vb_fixed_map->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2*29*29);
					}
					p_resource->p_vb_fixed_map->End();
#else
					p_resource->p_vb_tile->Begin();

					// �ٴ� �׸���
					{
						float x_offset = p_main_player->attribute.pos.x - floorf(p_main_player->attribute.pos.x);
						float y_offset = p_main_player->attribute.pos.y - floorf(p_main_player->attribute.pos.y);

						for (int y = -MAP_RANGE; y <= MAP_RANGE; y++)
						for (int x = -MAP_RANGE; x <= MAP_RANGE; x++)
						{
							int tile = p_resource->map.GetTile(x, y);

							TD3DMatrix mat_world;
							D3DXMatrixIdentity(&mat_world);
							D3DXMatrixTranslation(&mat_world, 1.0f * x - x_offset, 0.0f, 1.0f * y - y_offset);
							g_p_d3d_device->SetTransform(D3DTS_WORLD, &mat_world);

							{
								D3DXMatrixMultiply(&mat_wv, &mat_world, &mat_view);
								D3DXMatrixMultiply(&mat_wvp, &mat_wv, &mat_proj);

								D3DXMatrixTranspose(&mat_wvp, &mat_wvp);
								g_p_d3d_device->SetVertexShaderConstantF(4, (float*)&mat_wvp, 4);

								D3DXMatrixTranspose(&mat_wv, &mat_wv);
								g_p_d3d_device->SetVertexShaderConstantF(8, (float*)&mat_wv, 4);

								D3DXMatrixTranspose(&mat_world, &mat_world);
								g_p_d3d_device->SetVertexShaderConstantF(12, (float*)&mat_world, 4);
							}

							p_resource->p_vb_tile->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, tile*NUM_OF_VERTICES, 10);
							//p_resource->p_vb_temp->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
						}
					}
					p_resource->p_vb_tile->End();
#endif
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

					p_resource->p_vb_tile->Begin();

					// ��� �׸���
					{
						for (int y = -MAP_RANGE; y <= MAP_RANGE; y++)
						for (int x = -MAP_RANGE; x <= MAP_RANGE; x++)
						{
							int wall = p_resource->map.GetWall(x, y);
							if (wall == 0)
								continue;

							TD3DMatrix mat_world;
							D3DXMatrixIdentity(&mat_world);
							D3DXMatrixTranslation(&mat_world, 1.0f * x - p_main_player->attribute.pos.x, 0.26f, 1.0f * y - p_main_player->attribute.pos.y);
							g_p_d3d_device->SetTransform(D3DTS_WORLD, &mat_world);

							{
								D3DXMatrixMultiply(&mat_wv, &mat_world, &mat_view);
								D3DXMatrixMultiply(&mat_wvp, &mat_wv, &mat_proj);

								D3DXMatrixTranspose(&mat_wvp, &mat_wvp);
								g_p_d3d_device->SetVertexShaderConstantF(4, (float*)&mat_wvp, 4);

								D3DXMatrixTranspose(&mat_wv, &mat_wv);
								g_p_d3d_device->SetVertexShaderConstantF(8, (float*)&mat_wv, 4);

								D3DXMatrixTranspose(&mat_world, &mat_world);
								g_p_d3d_device->SetVertexShaderConstantF(12, (float*)&mat_world, 4);
							}

							p_resource->p_vb_tile->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, wall*NUM_OF_VERTICES, 10);
						}
					}
					p_resource->p_vb_tile->Flush();
					p_resource->p_vb_tile->End();
				}

				s_SetupMatrices();
			}

			DisplayMainCharacter(0);
			DisplayMainCharacter(1);

			g_p_d3d_device->EndScene();
		}
	}

	void DisplayUi(void)
	{
		gfx::SetFlatMode();

		if (_SUCCEEDED(g_p_d3d_device->BeginScene()))
		{
			p_old_map_data->DisplayStage(STAGE);

			// temporary icon for exit
			erio::gfx::FillRect(0x80FFFFFF, 800 - 60 - 20, 20, 60, 60);
			erio::gfx::FillRect(0xFFFF0000, 800 - 20 - 40, 40, 20, 20);

			g_p_d3d_device->EndScene();
		}
	}

} // namespace

namespace erio
{
	namespace game_play
	{
		bool OnCreate(unsigned long param);
		bool OnDestory(void);
		bool OnProcess(void);

		AppCallback callback =
		{
			OnCreate,
			OnDestory,
			OnProcess
		};
	} // namespace game_play
} // namespace erio


bool erio::game_play::OnCreate(unsigned long param)
{
	p_old_map_data = new COldMapData("STAGE4.MAP");

	p_resource = new TResource;

	// stage map making
	{
		const int STAGE_W_SIZE = 17;
		const int STAGE_H_SIZE = 17;

		char _MAP_DATA[STAGE_H_SIZE][STAGE_W_SIZE+1];

		const char* MAP_DATA[STAGE_H_SIZE+1] =
		{
			_MAP_DATA[0],
			_MAP_DATA[1],
			_MAP_DATA[2],
			_MAP_DATA[3],
			_MAP_DATA[4],
			_MAP_DATA[5],
			_MAP_DATA[6],
			_MAP_DATA[7],
			_MAP_DATA[8],
			_MAP_DATA[9],
			_MAP_DATA[10],
			_MAP_DATA[11],
			_MAP_DATA[12],
			_MAP_DATA[13],
			_MAP_DATA[14],
			_MAP_DATA[15],
			_MAP_DATA[16],
			NULL
		};

		int pc_start_x = 0;
		int pc_start_y = 0;

		for (int y = 0; y < STAGE_H_SIZE; y++)
		{
			for (int x = 0; x < STAGE_W_SIZE; x++)
			{
				COldMapData::TFixedObject fixed_object = p_old_map_data->GetMapData(STAGE, x, y).fixed_object;
				COldMapData::TFieldObject field_object = p_old_map_data->GetMapData(STAGE, x, y).field_object;

				int ix_map = 0;

				switch (fixed_object)
				{
				case COldMapData::FIXEDOBJECT_NORMAL:
					ix_map = 0;
					break;
				case COldMapData::FIXEDOBJECT_MINE:
					ix_map = 2;
					break;
				case COldMapData::FIXEDOBJECT_WATER:
					ix_map = 4;
					break;
				case COldMapData::FIXEDOBJECT_RECLAIM:
					ix_map = 0;
					break;
				case COldMapData::FIXEDOBJECT_WALL:
					ix_map = 1;
					break;
				case COldMapData::FIXEDOBJECT_DECORATION:
					ix_map = 0;
					break;
				}

				int ix_object = 0;

				switch (field_object)
				{
				case COldMapData::FIELDOBJECT_NONE:
					ix_object = 0;
					break;
				case COldMapData::FIELDOBJECT_BLOCK:
					ix_object = 5;
					break;
				case COldMapData::FIELDOBJECT_GOAL_BLOCK:
				case COldMapData::FIELDOBJECT_GOAL_PC:
					ix_object = 0;
					break;
				case COldMapData::FIELDOBJECT_PC:
					pc_start_x = x;
					pc_start_y = STAGE_H_SIZE - y;
					break;
				}

				if (ix_map)
					_MAP_DATA[y][x] = '0' + ix_map;
				else
					_MAP_DATA[y][x] = '0' + ix_object;
			}

			_MAP_DATA[y][STAGE_W_SIZE] = 0;
		}

		int map_offset_x = (STAGE_W_SIZE / 2);
		int map_offset_y = (STAGE_H_SIZE / 2);

		p_resource->map.Init(MAP_DATA, -map_offset_x, -map_offset_y);

		pc_start_x -= map_offset_x;
		pc_start_y -= map_offset_y;

		p_resource->player_list.push_back(CreateCharacter(0, float(pc_start_x), float(pc_start_y) - 0.5f));
	}

	p_resource->p_d3d_attrib = new CSm3DAttrib(g_p_d3d_device, 800.0f / 480.0f);

	p_resource->sprite = iu::shared_ptr<CTexture>(new CTexture(g_p_d3d_device, "NewNeto1_512_256.tga"));

	if (p_resource->sprite->m_p_texture == 0)
	{
		p_resource->sprite = iu::shared_ptr<CTexture>(new CTexture(g_p_d3d_device, "res_Block/NewNeto1_512_256.tga"));
	}

	/*
		>> way to extract 'IGfxSurface' from 'CTexture'
		avej_lite::IGfxSurface* p_res_sprite = p_resource->sprite->m_p_texture->m_p_surface;
	*/

	const int MAX_FACE_INC = 8;
	{
		struct TFRect
		{
			float x1, y1, x2, y2;
		};

		const TFRect src_rect[MAX_FACE_INC] =
		{
			// ^
			{261.0f / 512.0f,  59.0f / 256.0f, 298.0f / 512.0f, 116.0f / 256.0f},
			{299.0f / 512.0f,  59.0f / 256.0f, 336.0f / 512.0f, 116.0f / 256.0f},
			// _
			{298.0f / 512.0f,  59.0f / 256.0f, 261.0f / 512.0f, 116.0f / 256.0f},
			{336.0f / 512.0f,  59.0f / 256.0f, 299.0f / 512.0f, 116.0f / 256.0f},
			// ->
			{298.0f / 512.0f,   1.0f / 256.0f, 261.0f / 512.0f,  58.0f / 256.0f},
			{336.0f / 512.0f,   1.0f / 256.0f, 299.0f / 512.0f,  58.0f / 256.0f},
			// <-
			{261.0f / 512.0f,   1.0f / 256.0f, 298.0f / 512.0f,  58.0f / 256.0f},
			{299.0f / 512.0f,   1.0f / 256.0f, 336.0f / 512.0f,  58.0f / 256.0f},
		};

		TVertexBuffer<VERTEXFORMAT_POS_TEX>::TVertexData vertices[MAX_FACE_INC][4];

		for (int i = 0; i < MAX_FACE_INC; i++)
		{
			vertices[i][0].position = D3DVECTOR3(-0.5f, 1.0f, 0.0f);
			vertices[i][0].tex_pos.x = src_rect[i].x1;
			vertices[i][0].tex_pos.y = src_rect[i].y1;
			vertices[i][1].position = D3DVECTOR3( 0.5f, 1.0f, 0.0f);
			vertices[i][1].tex_pos.x = src_rect[i].x2;
			vertices[i][1].tex_pos.y = src_rect[i].y1;
			vertices[i][2].position = D3DVECTOR3(vertices[i][0].position.x, 0.0f, vertices[i][0].position.z);
			vertices[i][2].tex_pos.x = src_rect[i].x1;
			vertices[i][2].tex_pos.y = src_rect[i].y2;
			vertices[i][3].position = D3DVECTOR3(vertices[i][1].position.x, 0.0f, vertices[i][1].position.z);
			vertices[i][3].tex_pos.x = src_rect[i].x2;
			vertices[i][3].tex_pos.y = src_rect[i].y2;
		}

		p_resource->p_vb_character = new TVertexBuffer<VERTEXFORMAT_POS_TEX>(g_p_d3d_device, &vertices[0][0], MAX_FACE_INC, 4, p_resource->sprite);
		p_resource->p_vb_character->SetShader(p_resource->shader_set[VERTEXFORMAT_POS_TEX]);
	}

	{
		TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>::TVertexData vertices[7][NUM_OF_VERTICES];

		s_ConvertIsometricToVertice(vertices[0], NUM_OF_VERTICES, TBounds( 66.0f, 105.0f, 130.0f, 138.0f), p_resource->sprite.get()->m_p_texture);
		s_ConvertIsometricToVertice(vertices[1], NUM_OF_VERTICES, TBounds( 66.0f, 139.0f, 130.0f, 174.0f), p_resource->sprite.get()->m_p_texture);
		s_ConvertIsometricToVertice(vertices[2], NUM_OF_VERTICES, TBounds( 66.0f,   1.0f, 130.0f, 104.0f), p_resource->sprite.get()->m_p_texture);
		s_ConvertIsometricToVertice(vertices[3], NUM_OF_VERTICES, TBounds(131.0f,   1.0f, 195.0f, 104.0f), p_resource->sprite.get()->m_p_texture);
		s_ConvertIsometricToVertice(vertices[4], NUM_OF_VERTICES, TBounds(199.0f,   1.0f, 257.0f,  57.0f), p_resource->sprite.get()->m_p_texture);
		s_ConvertIsometricToVertice(vertices[5], NUM_OF_VERTICES, TBounds(199.0f,  58.0f, 257.0f, 111.0f), p_resource->sprite.get()->m_p_texture);

		short indice[10][3] =
		{
			{ 1, 0, 2}, { 2, 0, 3},
			{ 4, 5, 6}, { 6, 5, 7},
			{ 6, 7, 8}, { 8, 7, 9},
			{ 8, 9,10}, {10, 9,11},
			{10,11,12}, {12,11,13},
		};
		
		p_resource->p_vb_tile = new TVertexIndexBuffer<VERTEXFORMAT_POS_NOR_TEX>(g_p_d3d_device, &vertices[0][0], 7, NUM_OF_VERTICES, &indice[0][0], 30, p_resource->sprite);
		p_resource->p_vb_tile->SetShader(p_resource->shader_set[VERTEXFORMAT_POS_NOR_TEX]);

		{
			const int RADIUS_WIDTH  = 14;
			const int RADIUS_HEIGHT = 14;
			TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>::TVertexData vertices_fm[2*RADIUS_WIDTH+1][2*RADIUS_HEIGHT+1][2*3];

			for (int y = -RADIUS_HEIGHT; y <= RADIUS_HEIGHT; y++)
			for (int x = -RADIUS_WIDTH; x <= RADIUS_WIDTH; x++)
			{
				float x_base = 1.0f * x;
				float y_base = 1.0f * y;

				TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>::TVertexData* p_vertext_data = &vertices_fm[x+RADIUS_WIDTH][y+RADIUS_HEIGHT][0];

				for (int i = 0; i < 2; i++)
				{
					int tile = p_resource->map.GetTile(x, y);

					p_vertext_data[3*i+0] = vertices[tile][indice[i][0]];
					p_vertext_data[3*i+1] = vertices[tile][indice[i][1]];
					p_vertext_data[3*i+2] = vertices[tile][indice[i][2]];

					p_vertext_data[3*i+0].position.x += x_base;
					p_vertext_data[3*i+0].position.z += y_base;
					p_vertext_data[3*i+1].position.x += x_base;
					p_vertext_data[3*i+1].position.z += y_base;
					p_vertext_data[3*i+2].position.x += x_base;
					p_vertext_data[3*i+2].position.z += y_base;
				}
			}

			p_resource->p_vb_fixed_map = new TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>(g_p_d3d_device, &vertices_fm[0][0][0], (2*RADIUS_WIDTH+1) * (2*RADIUS_HEIGHT+1), 2*3, p_resource->sprite);
			p_resource->p_vb_fixed_map->SetShader(p_resource->shader_set[VERTEXFORMAT_POS_NOR_TEX]);
		}
	}

	{
		TVertexBuffer<VERTEXFORMAT_POS_DIF>::TVertexData vertices[2][3] =
		{
			{
				{ D3DVECTOR3(0.0f, 0.0f, 0.0f), { 1.0f, 0.0f, 0.0f, 1.0f } },
				{ D3DVECTOR3(0.0f, 0.0f, 1.0f), { 0.0f, 1.0f, 0.0f, 1.0f } },
				{ D3DVECTOR3(1.0f, 0.0f, 0.0f), { 0.0f, 0.0f, 1.0f, 1.0f } }
			},
			{
				{ D3DVECTOR3(1.0f, 0.0f, 0.0f), { 0.0f, 0.0f, 1.0f, 1.0f } },
				{ D3DVECTOR3(0.0f, 0.0f, 1.0f), { 0.0f, 1.0f, 0.0f, 1.0f } },
				{ D3DVECTOR3(1.0f, 0.0f, 1.0f), { 1.0f, 1.0f, 0.0f, 1.0f } }
			}
		};

		p_resource->p_vb_temp = new TVertexBuffer<VERTEXFORMAT_POS_DIF>(g_p_d3d_device, &vertices[0][0], 2, 3, p_resource->sprite);
		p_resource->p_vb_temp->SetShader(p_resource->shader_set[VERTEXFORMAT_POS_DIF]);
	}

#if 1
	s_SetUpGl();
#else
	g_p_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_p_d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_p_d3d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	g_p_d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
	g_p_d3d_device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	g_p_d3d_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	g_p_d3d_device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	g_p_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1,     D3DTA_TEXTURE);
	g_p_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG2,     D3DTA_DIFFUSE);
	g_p_d3d_device->SetTextureStageState(0, D3DTSS_COLOROP,       D3DTOP_MODULATE);
	g_p_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1,     D3DTA_TEXTURE);
	g_p_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2,     D3DTA_DIFFUSE);
	g_p_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAOP,       D3DTOP_MODULATE);
#endif

	return true;
}

bool erio::game_play::OnDestory(void)
{
	delete p_resource;
	delete p_old_map_data;

	return true;
}

bool erio::game_play::OnProcess(void)
{
	CInputDevice& input_device = avej_lite::singleton<CInputDevice>::get();
	input_device.UpdateInputState();

	if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_SYS1))
	{
		g_ChangeState(STATE_EXIT);
		return false;
	}

	ProcessObjects(input_device);

	DisplayObjects();
	DisplayUi();

	g_p_d3d_device->Present(NULL, NULL, 0, NULL);

	return true;
}
