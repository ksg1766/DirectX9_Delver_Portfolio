#pragma once
#include "Base.h"
#include "Export_Function.h"

BEGIN(Engine)

class CTempCamera;
class ENGINE_DLL CFrustum :
    public CBase
{
public:
	explicit CFrustum();
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFrustum(const CFrustum& rhs);
	virtual ~CFrustum();

public:
	BOOL	MakeFrustum(_matrix* _pmatViewProj);
	_int	Update_Frustum();

	BOOL	IsIn(_vec3& pv);
	BOOL	IsInSphere(_vec3& pv, _float radius);

	_vec3*	GetPos() { return &m_vPos; }

private:
	_vec3		m_vtx[8];
	_vec3		m_vPos;
	D3DXPLANE	m_tPlane[6];

	CTempCamera* m_pCamera = nullptr;

public:
	virtual void Free() override;
};

END