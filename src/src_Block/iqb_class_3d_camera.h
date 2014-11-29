
#ifndef __IQB_CLASS_3D_CAMERA_H__
#define __IQB_CLASS_3D_CAMERA_H__

#include "iqb_class_3d.h"

namespace erio
{
	class CSm3DCamera: public I3dActor
	{
	public:
		CSm3DCamera(IDirect3DDevice9* p_d3d_device);

		unsigned long Process(long ref_time = 0, I3dActor* p_sender = 0);

		single GetAngle(void)
		{
			return m_angle;
		}
		
		void SetAngle(single angle)
		{
			m_SetAngle(angle);
		}

		single GetHeight(void)
		{
			return m_height;
		}

		void SetHeight(single height)
		{
			m_SetHeight(height);
		}

		single GetRadius(void)
		{
			return m_radius;
		}

		void SetRadius(single radius)
		{
			m_SetRadius(radius); 
		}

	private:
		IDirect3DDevice9* m_p_d3d_device;
		bool              m_modified;
		TD3DVector3       m_vec_eye;
		TD3DVector3       m_vec_look_at;
		TD3DVector3       m_vec_up;
		single            m_angle;
		single            m_height;
		single            m_radius;

		void m_SetAngle(single angle);
		void m_SetHeight(single height);
		void m_SetRadius(single radius);
		void m_Apply(void);
	};

} // namespace erio

#endif // #ifndef __IQB_CLASS_3D_CAMERA_H__
