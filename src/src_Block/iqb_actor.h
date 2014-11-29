
#ifndef __IQB_ACTOR_H__
#define __IQB_ACTOR_H__

#include "iqb_base_type.h"

namespace erio
{
	class CSmPlayer: public ISmActor
	{
	public:
		CSmPlayer();
		~CSmPlayer();

		void Move(float dx, float dy, bool turn_face = true);
		void Warp(float x, float y);
		unsigned long Process(long ref_time, ISmActor* p_sender = 0);

		struct TAttribute
		{
			TSmPointEx pos;
			int face;
			int face_inc;
			int face_inc_count;

			TAttribute()
				: face(0)
				, face_inc(0)
				, face_inc_count(0)
			{
			}
		} attribute;
	};

	CSmPlayer* CreateCharacter(int type, float x, float y);

} // namespace erio

#endif // #ifndef __IQB_ACTOR_H__
