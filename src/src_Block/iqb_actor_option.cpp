
#include "iqb_z_config.h"
#include "iqb_base_gfx.h"

using namespace avej_lite;

namespace erio
{

namespace option
{
	bool OnCreate(unsigned long param)
	{
		return true;
	}

	bool OnDestory(void)
	{
		return true;
	}

	bool OnProcess(void)
	{
		// immediately terminate
		return false;
	}

	AppCallback callback =
	{
		OnCreate,
		OnDestory,
		OnProcess
	};
}

} // namespace erio
