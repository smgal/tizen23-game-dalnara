
#ifndef __AVEJAPP_REGISTER_H__
#define __AVEJAPP_REGISTER_H__

////////////////////////////////////////////////////////////////////////////////
// CAppRegister

struct TAppCallback
{
	bool (*Initialize)(void* h_window);
	void (*Finalize)  (void);
	bool (*Process)   (void);

	TAppCallback()
		: Initialize(0), Finalize(0), Process(0) {}
};

class CAppRegister
{
public:
	typedef bool (*FnGetApp)(TAppCallback&);

	CAppRegister(const char* sz_app_name, FnGetApp fn_get_app);

	static FnGetApp* GetAppByName(const char* sz_app_name);
};

#define REGISTER_APP(name, func, num) static CAppRegister s_register_app_##num(name, func);

#define REGISTER_APP_1(name, func) REGISTER_APP(name, func, 1);
#define REGISTER_APP_2(name, func) REGISTER_APP(name, func, 2);
#define REGISTER_APP_3(name, func) REGISTER_APP(name, func, 3);
#define REGISTER_APP_4(name, func) REGISTER_APP(name, func, 4);

#endif // #ifndef __AVEJAPP_REGISTER_H__
