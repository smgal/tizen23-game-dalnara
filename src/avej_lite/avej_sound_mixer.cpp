
#include "avej_sound_mixer.h"

struct avej_lite::sound::CMixer::TImpl
{
	int dummy;
};

avej_lite::sound::CMixer::CMixer()
: m_p_impl(0)
{
}

avej_lite::sound::CMixer::~CMixer()
{
	delete m_p_impl;
}

bool avej_lite::sound::CMixer::Load(int sound, const char* sz_file_name)
{
	return false;
}

bool avej_lite::sound::CMixer::Load(int sound, const void* p_buffer, int buffer_size)
{
	return false;
}

void avej_lite::sound::CMixer::Play(unsigned voice, unsigned sound, int left_vol, int right_vol)
{
}
