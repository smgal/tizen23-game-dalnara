
#include "../avej_config.h"

extern int AvejMain(void);

#if (TARGET_DEVICE == TARGET_TIZEN)
int __main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	ENABLE_MEMORY_MANAGER
//	_CrtSetBreakAlloc(??);

	return AvejMain();
}
