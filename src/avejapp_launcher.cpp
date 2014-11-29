
////////////////////////////////////////////////////////////////////////////////
// Application list

const char* G_APP_LIST[] =
{
	"DALNARA",
	"ShitFighter_Wave",
	"ManoEri",
	0
};

namespace
{
	const char** s_GetAppList(void)
	{
		return G_APP_LIST;
	}
}

void GetAppList(const char* app_list[], int num_alloc)
{
	const char** p_sz_app_list = s_GetAppList();
	const char** p_sz_app_list_end = p_sz_app_list;

	const char** p_sz_out_list = app_list;

	{
		while (*p_sz_app_list_end && num_alloc-- > 0)
			*p_sz_out_list++ = *p_sz_app_list_end++;

		if (num_alloc > 0)
			*p_sz_out_list++ = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
// uses

#include "avejapp_register.h"

////////////////////////////////////////////////////////////////////////////////
// AvejMain()

int AvejMain(void)
{
	CAppRegister::FnGetApp* FnGetAppCallback = CAppRegister::GetAppByName(G_APP_LIST[0]);

	if (FnGetAppCallback)
	{
		TAppCallback app_callback;

		if ((*FnGetAppCallback)(app_callback))
		{
			app_callback.Initialize(0);

			while (app_callback.Process())
				;

			app_callback.Finalize();
		}
	}

	return 0;
}
