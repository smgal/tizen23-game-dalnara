
#include "iqb_actor.h"
#include <assert.h>

namespace
{
	template <class T>
	inline int Sign(T data)
	{
		return (data == 0) ? 0 : ((data > 0) ? 1 : -1);
	}
}

erio::CSmPlayer::CSmPlayer()
{
}

erio::CSmPlayer::~CSmPlayer()
{
}

void erio::CSmPlayer::Move(float dx, float dy, bool turn_face)
{
	attribute.pos.x += dx;
	attribute.pos.y += dy;

	if (turn_face)
	{
		if (dx == 0 && dy == 0)
			return;

		int index = (2-(Sign(dy)+1)) * 3 + (Sign(dx)+1);

		const int FACE_DATA[9] =
		{
			0, 0, 1,
			3, 0, 1,
			3, 2, 2
		};

		assert(index >= 0 && index < 9);

		attribute.face = FACE_DATA[index];
	}

	++attribute.face_inc_count;

	if (attribute.face_inc_count > 10)
	{
		attribute.face_inc_count = 0;
		attribute.face_inc = (attribute.face_inc+1) % 2;
	}
}

void erio::CSmPlayer::Warp(float x, float y)
{
	attribute.pos.x = x;
	attribute.pos.y = y;
}

unsigned long erio::CSmPlayer::Process(long ref_time, ISmActor* p_sender)
{
	return 0;
}

erio::CSmPlayer* erio::CreateCharacter(int type, float x, float y)
{
	CSmPlayer* p_player = new CSmPlayer;

	p_player->Warp(x, y);
	
	return p_player;
}
