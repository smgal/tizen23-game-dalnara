
#include "avejapp_register.h"

struct TAppList
{
	typedef CAppRegister::FnGetApp FnGetApp;

	void      RegisterApp(const char* sz_app_name, FnGetApp fn_get_app);
	FnGetApp* GetAppFunc(const char* sz_app_name);

private:
	char     m_name[32];
	FnGetApp m_fn_get_app;
};

void TAppList::RegisterApp(const char* sz_app_name, FnGetApp fn_get_app)
{
	if (fn_get_app == 0)
		return;

	const char* p_src = sz_app_name;
	char*       p_dst = &this->m_name[0];

	for (int i = 0; i < sizeof(this->m_name); i++)
	{
		if (*p_src == 0)
		{
			*p_dst++ = 0;

			for (int j = i+1; j < sizeof(this->m_name); j++)
				*p_dst++ = 0;

			break;
		}
		*p_dst++ = *p_src++;
	}

	this->m_fn_get_app = fn_get_app;
}

TAppList::FnGetApp* TAppList::GetAppFunc(const char* sz_app_name)
{
	if (this->m_fn_get_app == 0)
		return 0;

	const char* p_src = sz_app_name;
	const char* p_dst = &this->m_name[0];

	for (int i = 0; i < sizeof(this->m_name); i++)
	{
		if (*p_src == 0 && *p_dst == 0)
			return &this->m_fn_get_app;

		if (*p_dst++ != *p_src++)
			return 0;
	}

	// NULL없이 32자를 채웠을 경우
	return &this->m_fn_get_app;
}

////////////////////////////////////////////////////////////////////////////////

const int MAX_APP = 30;

static TAppList S_REGISTERED_APP[MAX_APP];
static int      s_num_registered_app = 0;

CAppRegister::CAppRegister(const char* sz_app_name, FnGetApp fn_get_app)
{
	if (s_num_registered_app < MAX_APP)
	{
		S_REGISTERED_APP[s_num_registered_app++].RegisterApp(sz_app_name, fn_get_app);
	}
}

CAppRegister::FnGetApp* CAppRegister::GetAppByName(const char* sz_app_name)
{
	TAppList* p_app_list = &S_REGISTERED_APP[0];
	TAppList* p_app_list_end = p_app_list + s_num_registered_app;

	while (p_app_list < p_app_list_end)
	{
		FnGetApp* p_fn_get_app = p_app_list->GetAppFunc(sz_app_name);

		if (p_fn_get_app)
			return p_fn_get_app;

		++p_app_list;
	}

	return 0;
}
