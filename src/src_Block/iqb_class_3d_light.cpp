
#include "iqb_class_3d_light.h"
#include "iqb_class_3d_shader.h"

erio::CSm3DLight::CSm3DLight(IDirect3DDevice9* p_d3d_device)
{
	m_p_d3d_device = p_d3d_device;

	memset(&m_light, 0, sizeof(m_light));

	m_light.Type      = D3DLIGHT_DIRECTIONAL;
	m_light.Diffuse.r = 0.6f;
	m_light.Diffuse.g = 0.6f;
	m_light.Diffuse.b = 0.6f;
	m_light.Range     = 1000.0;

	TD3DVector3 vec_direction = D3DVECTOR3(-5.0, -5.0, 5.0);

	D3DXVec3Normalize((TD3DVector3*)&m_light.Direction, &vec_direction);

	m_p_d3d_device->LightEnable(0, true);
	m_p_d3d_device->SetRenderState(D3DRS_LIGHTING, 1);

	m_p_d3d_device->SetRenderState(D3DRS_AMBIENT, 0x00606060);

	{
		shader::SetLightDiffuse(m_p_d3d_device, 0.6f, 0.6f, 0.6f);
		shader::SetLightAmbient(m_p_d3d_device, float(0x60) / 255.0f, float(0x60) / 255.0f, float(0x60) / 255.0f);
	}

	m_Apply();
}

unsigned long erio::CSm3DLight::Process(long ref_time, I3dActor* p_sender)
{
	m_p_d3d_device->LightEnable(0, true);
	m_p_d3d_device->SetRenderState(D3DRS_LIGHTING, 1);

	m_Apply();

	return 0;
}

void erio::CSm3DLight::m_Apply(void)
{
	m_p_d3d_device->SetLight(0, &m_light);

	shader::SetLightDir(m_p_d3d_device, m_light.Direction.x, m_light.Direction.y, m_light.Direction.z);
}
