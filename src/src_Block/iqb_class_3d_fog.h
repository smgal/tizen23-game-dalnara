
#ifndef __IQB_CLASS_3D_FOG_H__
#define __IQB_CLASS_3D_FOG_H__

#include "iqb_class_3d.h"

namespace erio
{
	const float FOG_START = 18.0f * 1.0f;
	const float FOG_END   = 30.0f * 1.0f;

	class CSm3DFog: public I3dActor
	{
	public:
		CSm3DFog(IDirect3DDevice9* p_d3d_device);

		unsigned long Process(long ref_time = 0, I3dActor* p_sender = 0);

	private:
		IDirect3DDevice9* m_p_d3d_device;
		single            m_fog_start;
		single            m_fog_end;

		void m_Apply(void);
	};

} // namespace erio

#endif // #ifndef __IQB_CLASS_3D_FOG_H__
