
////////////////////////////////////////////////////////////////////////////////
// uses

#include "iqb_base_gfx.h"
#include "iqb_class_3d.h"
#include "iqb_class_3d_vertext.h"
#include "iqb_class_3d_attrib.h"

#include <assert.h>

using namespace avej_lite;

namespace
{
	template <typename T>
	void s_Increase(T& value)
	{
		int temp = (int)value;
		value = (T)(temp+1);
	}

	void s_ClearKeyBuffer(void)
	{
		avej_lite::CInputDevice& input_device = avej_lite::singleton<avej_lite::CInputDevice>::get();
		input_device.UpdateInputState();

		// remove key buffer
		for (avej_lite::TInputKey key = avej_lite::INPUT_KEY_BEGIN; key < avej_lite::INPUT_KEY_END; s_Increase(key))
			input_device.WasKeyPressed(key);
	}
}

namespace erio
{
	///////////////////////////////////////////////////////////////////////////////
	// global

	IDirect3DDevice9* g_p_d3d_device = 0;

	///////////////////////////////////////////////////////////////////////////////
	// state machine

	namespace team_logo    { extern AppCallback callback; }
	namespace title        { extern AppCallback callback; }
	namespace option       { extern AppCallback callback; }
	namespace stage_select { extern AppCallback callback; }
	namespace game_play    { extern AppCallback callback; }
	namespace ending       { extern AppCallback callback; }

	namespace
	{
		bool          s_state_changing = false;
		TState        s_required_state = STATE_EXIT;
		unsigned long s_state_param1 = 0;

		const AppCallback* s_app_callback_list[STATE_EXIT+1] =
		{
			// STATE_TEAM_LOGO
			&team_logo::callback,
			// STATE_TITLE
			&title::callback,
			// STATE_OPTION
			&option::callback,
			// STATE_STAGE_SELECT
			&stage_select::callback,
			// STATE_GAME_PLAY
			&game_play::callback,
			// STATE_ENDING
			&ending::callback,
			//  STATE_EXIT
			NULL,
		};
	}

	void g_ChangeState(TState state, unsigned long param1)
	{
		s_required_state = state;
		s_state_param1 = param1;
		s_state_changing = true;
	}

	///////////////////////////////////////////////////////////////////////////////
	// CSystem class
	class CSystem
	{
		enum TProcess
		{
			PROCESS_READY,
			PROCESS_IN
		};

	public:
		CSystem(TState state)
		: m_current_state(state), m_process(PROCESS_READY), m_p_app(0)
		{
			TLayerDesc layer_option =
			{
				1,
				{
					avej_lite::TLayerDesc::LAYER_ATTRIB_ALPHA_TEST | avej_lite::TLayerDesc::LAYER_ATTRIB_ALPHA_BLEND
				},
				avej_lite::TLayerDesc::OPTION_NONE
			};

			// initialize the graphics system
			g_p_gfx_device = IGfxDevice::GetInstance();
			
			gfx::Init();

			g_p_gfx_device->SetLayerDesc(layer_option);
			g_p_gfx_device->GetSurface(&g_p_back_buffer);

			g_p_d3d_device = new IDirect3DDevice9(g_p_gfx_device);
		}

		~CSystem()
		{
			if (g_p_d3d_device)
			{
				g_p_d3d_device->Release();
				g_p_d3d_device = NULL;
			}

			g_p_gfx_device->Release();
		}

		bool Process(void)
		{
			switch (m_process)
			{
			case PROCESS_READY:
				{
					if (s_app_callback_list[m_current_state] == 0)
						return false;

					s_ClearKeyBuffer();

					assert(m_p_app == 0);
					m_p_app = IAvejApp::GetInstance(*s_app_callback_list[m_current_state], ::erio::s_state_param1);
					assert(m_p_app);

					m_process = PROCESS_IN;
				}
				// pass through
			case PROCESS_IN:
				{
					if (!m_p_app->Process())
					{
						m_p_app->Release();
						m_p_app = 0;

						if (s_state_changing)
						{
							m_current_state = s_required_state;
							s_state_changing = false;
						}

						if (s_required_state == STATE_EXIT)
							return false;

						m_process = PROCESS_READY;
					}
				}
				break;
			}

			return true;
		}

	protected:
		void _Run(TState state)
		{
			while (s_app_callback_list[state])
			{
				s_ClearKeyBuffer();

				IAvejApp* p_app = IAvejApp::GetInstance(*s_app_callback_list[state]);
				assert(p_app);
				
				p_app->Process();
				p_app->Release();

				if (s_state_changing)
				{
					state = s_required_state;
					s_state_changing = false;
				}

				if (s_required_state == STATE_EXIT)
					break;
			}
		}

	private:
		TProcess  m_process;
		TState    m_current_state;
		IAvejApp* m_p_app;
	};

} // namespace erio

////////////////////////////////////////////////////////////////////////////////
// main

#include "avejapp_register.h"

using namespace erio;

namespace
{
	CSystem* p_system = 0;

	bool s_Initialize(void* h_window)
	{
	#if (TARGET_DEVICE == TARGET_TIZEN)
		avej_lite::util::SetResourcePath("res_DALNARA/");
	#elif (TARGET_DEVICE == TARGET_BADA)
		avej_lite::util::SetResourcePath("res_DALNARA/");
	#else
		avej_lite::util::SetResourcePath("../../Res/res_DALNARA/");
	#endif

		p_system = new CSystem(STATE_GAME_PLAY);

		return (p_system != 0);
	}

	void s_Finalize(void)
	{
		delete p_system;
	}

	bool s_Process(void)
	{
		return p_system->Process();
	}
}


static bool GetAppCallback(TAppCallback& out_callback)
{
	out_callback.Initialize = s_Initialize;
	out_callback.Finalize   = s_Finalize;
	out_callback.Process    = s_Process;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// main

bool g_Game_Initialize(void)
{
	return s_Initialize(0);
}

void g_Game_Finalize(void)
{
	s_Finalize();
}

bool g_Game_Process(void)
{
	return s_Process();
}

////////////////////////////////////////////////////////////////////////////////
//

REGISTER_APP_1("DALNARA", GetAppCallback);
