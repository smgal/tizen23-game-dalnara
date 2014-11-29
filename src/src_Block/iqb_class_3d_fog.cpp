
#include "iqb_class_3d_fog.h"
#include "iqb_class_3d_shader.h"

erio::CSm3DFog::CSm3DFog(IDirect3DDevice9* p_d3d_device)
{
	m_p_d3d_device = p_d3d_device;

	m_p_d3d_device->SetRenderState(D3DRS_FOGCOLOR, 0x00000000);
	m_p_d3d_device->SetRenderState(D3DRS_FOGENABLE, 1);

	m_p_d3d_device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	
	m_p_d3d_device->SetRenderState(D3DRS_FOGSTART, *((DWORD*)(&FOG_START)));
	m_p_d3d_device->SetRenderState(D3DRS_FOGEND, *((DWORD*)(&FOG_END)));

	m_p_d3d_device->SetRenderState(D3DRS_RANGEFOGENABLE, 1);

	shader::SetFog(m_p_d3d_device, FOG_START, FOG_END);

	m_Apply();
}

unsigned long erio::CSm3DFog::Process(long ref_time, I3dActor* p_sender)
{
	m_p_d3d_device->SetRenderState(D3DRS_FOGENABLE, 1);

	m_Apply();

	return 0;
}

void erio::CSm3DFog::m_Apply(void)
{
}
