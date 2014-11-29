
#include "iqb_class_3d_attrib.h"

erio::CSm3DAttrib::CSm3DAttrib(IDirect3DDevice9* p_d3d_device, float screen_ratio)
{
	// assign Direct3D device instance
	m_p_d3d_device = p_d3d_device;

	// initialize internal instances
	m_camera     = new CSm3DCamera(p_d3d_device);
	m_projection = new CSm3DProjection(p_d3d_device, screen_ratio);
	m_light      = new CSm3DLight(p_d3d_device);
	m_fog        = new CSm3DFog(p_d3d_device);
}

erio::CSm3DAttrib::~CSm3DAttrib(void)
{
	delete m_camera;
	delete m_projection;
	delete m_light;
	delete m_fog;
}

unsigned long erio::CSm3DAttrib::Process(long ref_time, I3dActor* p_sender)
{
	m_fog->Process(ref_time, p_sender);
	m_light->Process(ref_time, p_sender);
	m_camera->Process(ref_time, p_sender);
	m_projection->Process(ref_time, p_sender);

	return 0;
}
