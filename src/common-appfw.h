#ifndef __common_appfw_H__
#define __common_appfw_H__

class AppFw
{
public:
	struct AppDesc
	{
		char res_path[1024];
		// void (*terminate_cb)(void);
	};

	struct BufferDesc
	{
		void* p_start_address;
		int   width;
		int   height;
		int   bytes_per_line;
		int   bits_per_pixel;
		void* native_handle;
	};

	virtual ~AppFw() {}

	virtual void onCreate(const AppDesc& app_desc) = 0;
	virtual void onDestroy(void) = 0;
	virtual void onProcess(const BufferDesc& buffer_desc) = 0;

	virtual void onTouchDown(int x, int y) = 0;
	virtual void onTouchUp(int x, int y) = 0;
	virtual void onTouchMove(int x, int y) = 0;

	virtual bool inTerminating(void) = 0;

	static AppFw* create(void);
};

#endif
