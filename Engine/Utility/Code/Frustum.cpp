#include "Frustum.h"
#include "TempCamera.h"

#define PLANE_EPSILON	26.f

CFrustum::CFrustum()
{
	m_pCamera = dynamic_cast<CTempCamera*>(SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front());
}

CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphicDev)
{
}

CFrustum::CFrustum(const CFrustum& rhs)
{
}

CFrustum::~CFrustum()
{
}

BOOL CFrustum::MakeFrustum(_matrix* _pmatViewProj)
{
	_matrix	matInv;

	m_vtx[0].x = -1.0f; m_vtx[0].y = -1.0f; m_vtx[0].z = 0.0f;
	m_vtx[1].x =  1.0f; m_vtx[1].y = -1.0f; m_vtx[1].z = 0.0f;
	m_vtx[2].x =  1.0f; m_vtx[2].y = -1.0f; m_vtx[2].z = 1.0f;
	m_vtx[3].x = -1.0f; m_vtx[3].y = -1.0f; m_vtx[3].z = 1.0f;
	m_vtx[4].x = -1.0f; m_vtx[4].y =  1.0f; m_vtx[4].z = 0.0f;
	m_vtx[5].x =  1.0f; m_vtx[5].y =  1.0f; m_vtx[5].z = 0.0f;
	m_vtx[6].x =  1.0f; m_vtx[6].y =  1.0f; m_vtx[6].z = 1.0f;
	m_vtx[7].x = -1.0f; m_vtx[7].y =  1.0f; m_vtx[7].z = 1.0f;

	D3DXMatrixInverse(&matInv, NULL, _pmatViewProj);

	for (_int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vtx[i], &m_vtx[i], &matInv);

	D3DXPlaneFromPoints(&m_tPlane[0], &m_vtx[4], &m_vtx[7], &m_vtx[6]);
	D3DXPlaneFromPoints(&m_tPlane[1], &m_vtx[0], &m_vtx[1], &m_vtx[2]);
	D3DXPlaneFromPoints(&m_tPlane[2], &m_vtx[0], &m_vtx[4], &m_vtx[5]);
	D3DXPlaneFromPoints(&m_tPlane[3], &m_vtx[2], &m_vtx[6], &m_vtx[7]);
	D3DXPlaneFromPoints(&m_tPlane[4], &m_vtx[0], &m_vtx[3], &m_vtx[7]);
	D3DXPlaneFromPoints(&m_tPlane[5], &m_vtx[1], &m_vtx[5], &m_vtx[6]);

	return TRUE;
}

_int CFrustum::Update_Frustum()
{
	if (m_pCamera)
	{	
		if (!m_pCamera->Get_CameraMode())
		{
			_matrix _matViewProj = m_pCamera->Get_ViewMat() * m_pCamera->Get_ProjMat();
			//D3DXMatrixMultiply(&_matViewProj, &m_pCamera->Get_ViewMat(), &m_pCamera->Get_ProjMat());
			MakeFrustum(&_matViewProj);
			return 1;
		}
	}
	return 0;
}

BOOL CFrustum::IsIn(_vec3& pv)
{
	_float fDist;

	fDist = D3DXPlaneDotCoord(&m_tPlane[0], &pv);
	if (fDist > PLANE_EPSILON) return FALSE;
	fDist = D3DXPlaneDotCoord(&m_tPlane[1], &pv);
	if (fDist > PLANE_EPSILON) return FALSE;
	fDist = D3DXPlaneDotCoord(&m_tPlane[2], &pv);
	if (fDist > PLANE_EPSILON) return FALSE;
	fDist = D3DXPlaneDotCoord(&m_tPlane[3], &pv);
	if (fDist > PLANE_EPSILON) return FALSE;
	fDist = D3DXPlaneDotCoord(&m_tPlane[4], &pv);
	if (fDist > PLANE_EPSILON) return FALSE;
	fDist = D3DXPlaneDotCoord(&m_tPlane[5], &pv);
	if (fDist > PLANE_EPSILON) return FALSE;
	
	return TRUE;
}

BOOL CFrustum::IsInSphere(_vec3& pv, _float _fRadius)
{
	_float fDist;
	//_fRadius = 2.f;
	fDist = D3DXPlaneDotCoord(&m_tPlane[0], &pv);
	if (fDist > (_fRadius + PLANE_EPSILON)) return FALSE;
	fDist = D3DXPlaneDotCoord(&m_tPlane[1], &pv);
	if (fDist > (_fRadius + PLANE_EPSILON)) return FALSE;
	fDist = D3DXPlaneDotCoord(&m_tPlane[2], &pv);
	if (fDist > (_fRadius + PLANE_EPSILON)) return FALSE;
	fDist = D3DXPlaneDotCoord(&m_tPlane[3], &pv);
	if (fDist > (_fRadius + PLANE_EPSILON)) return FALSE;
	fDist = D3DXPlaneDotCoord(&m_tPlane[4], &pv);
	if (fDist > (_fRadius + PLANE_EPSILON)) return FALSE;
	fDist = D3DXPlaneDotCoord(&m_tPlane[5], &pv);
	if (fDist > (_fRadius + PLANE_EPSILON)) return FALSE;

	return TRUE;
}

void CFrustum::Free()
{

}
