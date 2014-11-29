
#ifndef __IQB_CLASS_3D_ATTRIB_H__
#define __IQB_CLASS_3D_ATTRIB_H__

#include "iqb_class_3d.h"
#include "iqb_class_3d_camera.h"
#include "iqb_class_3d_projection.h"
#include "iqb_class_3d_light.h"
#include "iqb_class_3d_fog.h"

namespace erio
{
	class CSm3DAttrib: public I3dActor
	{
	public:
		CSm3DAttrib(IDirect3DDevice9* p_d3d_device, float screen_ratio);
		~CSm3DAttrib(void);

		unsigned long Process(long ref_time = 0, I3dActor* p_sender = 0);

		inline CSm3DCamera* GetCamera(void) const
		{
			return m_camera;
		}

		inline CSm3DProjection* GetProjection(void) const
		{
			return m_projection;
		}

	private:
		IDirect3DDevice9* m_p_d3d_device;
		CSm3DCamera*      m_camera;
		CSm3DProjection*  m_projection;
		CSm3DLight*       m_light;
		CSm3DFog*         m_fog;

	};

} // namespace erio

#endif // #ifndef __IQB_CLASS_3D_ATTRIB_H__
