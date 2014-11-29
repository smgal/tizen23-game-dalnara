
#ifndef __IQB_CLASS_3D_PROJECTION_H__
#define __IQB_CLASS_3D_PROJECTION_H__

#include "iqb_class_3d.h"

namespace erio
{
	class CSm3DProjection: public I3dActor
	{
	public:
		enum
		{
			LOW_INDEX  = 0,
			HIGH_INDEX = 100,
		};
		enum TProjectionMode
		{
			PROJECTIONMODE_PERSPECTIVE,
			PROJECTIONMODE_ORTHOGONAL,
		};

	public:
		CSm3DProjection(IDirect3DDevice9* p_d3d_device, float screen_ratio);

		unsigned long Process(long ref_time = 0, I3dActor* p_sender = 0);

		TProjectionMode GetMode(void)
		{
			return m_projection_mode;
		}

		void SetMode(TProjectionMode mode)
		{
			m_SetProjectionMode(mode);
		}

	private:
		IDirect3DDevice9* m_p_d3d_device;
		TD3DMatrix        m_projection_matrix[HIGH_INDEX+1];
		TProjectionMode   m_projection_mode;
		int               m_projection_step;

		void m_InitiaizeProjectionMatrix(float screen_ratio);
		void m_SetProjectionMode(TProjectionMode mode);
		void m_Apply(void);
	};

} // namespace erio

#endif // #ifndef __IQB_CLASS_3D_PROJECTION_H__
