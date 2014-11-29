
#include "iqb_class_3d_camera.h"
#include <math.h>

erio::CSm3DCamera::CSm3DCamera(IDirect3DDevice9* p_d3d_device)
{
	m_p_d3d_device = p_d3d_device;

	m_modified = false;

	m_vec_look_at = D3DVECTOR3(0.0, 1.0, 0.0);

	if (USE_ROTATION)
		m_vec_up = D3DVECTOR3(-1.0, 0.0, 0.0);
	else
		m_vec_up = D3DVECTOR3(0.0, 1.0, 0.0);

	m_angle  = - 0.22f;
	m_height = 6.0;
	m_radius = 12.0;

	m_Apply();
}

unsigned long erio::CSm3DCamera::Process(long ref_time, I3dActor* p_sender)
{
	if (m_modified)
	{
		m_Apply();
		m_modified = false;
	}

	return 0;
}

void erio::CSm3DCamera::m_SetAngle(single angle)
{
	m_angle    = angle;
	m_modified = true;
}

void erio::CSm3DCamera::m_SetHeight(single height)
{
	m_height   = height;
	m_modified = true;
}

void erio::CSm3DCamera::m_SetRadius(single radius)
{
	m_radius   = radius;
	m_modified = true;
}

void erio::CSm3DCamera::m_Apply(void)
{
	TD3DMatrix mat_view;

	m_vec_eye = D3DVECTOR3(float(m_radius * sin(m_angle)), float(m_height), -float(m_radius * cos(m_angle)));

	D3DXMatrixLookAtLH(&mat_view, &m_vec_eye, &m_vec_look_at, &m_vec_up);

	m_p_d3d_device->SetTransform(D3DTS_VIEW, &mat_view);
}
