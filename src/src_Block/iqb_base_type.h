
#ifndef __IQB_BASE_TYPE_H__
#define __IQB_BASE_TYPE_H__

namespace erio
{
	struct TSmPoint
	{
		long x;
		long y;
	};

	struct TSmPointEx
	{
		float x;
		float y;

		TSmPointEx()
			: x(0.0f)
			, y(0.0f)
		{
		}

		TSmPointEx(float _x, float _y)
			: x(_x)
			, y(_y)
		{
		}
	};

	struct TSmRect
	{
		long  x, y, w, h;
		long  dx, dy;
	};

	class ISmActor
	{
	public:
		virtual ~ISmActor()
		{
		}

		virtual unsigned long Process(long ref_time = 0, ISmActor* p_sender = 0) = 0;
	};

} // namespace erio

#endif // #ifndef __IQB_BASE_TYPE_H__
