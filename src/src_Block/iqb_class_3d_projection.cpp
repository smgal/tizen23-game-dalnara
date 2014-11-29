
#include "iqb_class_3d_projection.h"
#include <math.h>

void erio::CSm3DProjection::m_InitiaizeProjectionMatrix(float screen_ratio)
{
	TD3DMatrix mat_project1;
	TD3DMatrix mat_project2;
	single     factor;

	if (screen_ratio == 0.0f)
		screen_ratio = 4.0f / 3.0f;

	if (USE_ROTATION)
		D3DXMatrixPerspectiveFovLH(&mat_project1, D3DX_PI / (6.0f / screen_ratio), 1.0f / screen_ratio, 1.0f, 100.0f);
	else
		D3DXMatrixPerspectiveFovLH(&mat_project1, D3DX_PI / 6.0f, screen_ratio, 1.0f, 100.0f);

	D3DXMatrixOrthoOffCenterLH(&mat_project2, -6.0f, 6.0f, -6.0f / screen_ratio, 6.0f / screen_ratio, 1.0f, 100.0f);

	for (int k = 0; k <= HIGH_INDEX; k++)
	{
		factor = k * 1.0f + 0.1f;
		factor = float(log(1.0 / factor) - 0.0);

		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				m_projection_matrix[k].m[i][j] = float(mat_project1.m[i][j] + (mat_project2.m[i][j] - mat_project1.m[i][j]) * factor / (log(1.0/HIGH_INDEX)-0.0));
			}
		}
	}

	m_projection_matrix[LOW_INDEX]  = mat_project1;
	m_projection_matrix[HIGH_INDEX] = mat_project2;
}

void erio::CSm3DProjection::m_SetProjectionMode(TProjectionMode mode)
{
	if (m_projection_mode == mode)
		return;

	++m_projection_step;
}

void erio::CSm3DProjection::m_Apply(void)
{
	TD3DMatrix matProject;

	if (m_projection_step > 0)
	{
		if (m_projection_mode == PROJECTIONMODE_PERSPECTIVE)
		{
			matProject = m_projection_matrix[m_projection_step];
		}
		else
		{
			matProject = m_projection_matrix[HIGH_INDEX-m_projection_step];
		}

		++m_projection_step;
		if (m_projection_step >= HIGH_INDEX)
		{
			m_projection_step = LOW_INDEX;
			if (m_projection_mode == PROJECTIONMODE_PERSPECTIVE)
				m_projection_mode = PROJECTIONMODE_ORTHOGONAL;
			else
				m_projection_mode = PROJECTIONMODE_PERSPECTIVE;
		}
	}
	else
	{
		if (m_projection_mode == PROJECTIONMODE_PERSPECTIVE)
			matProject = m_projection_matrix[LOW_INDEX];
		else
			matProject = m_projection_matrix[HIGH_INDEX];
	}

	m_p_d3d_device->SetTransform(D3DTS_PROJECTION, &matProject);
}

erio::CSm3DProjection::CSm3DProjection(IDirect3DDevice9* p_d3d_device, float screen_ratio)
{
	m_p_d3d_device = p_d3d_device;

	m_projection_mode = PROJECTIONMODE_PERSPECTIVE;
	m_projection_step = LOW_INDEX;

	m_InitiaizeProjectionMatrix(screen_ratio);

	m_Apply();
}

unsigned long erio::CSm3DProjection::Process(long ref_time, I3dActor* p_sender)
{
	m_Apply(); // if (m_projection_step > 0)

	return 0;
}
