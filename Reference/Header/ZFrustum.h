#pragma once
#include "Base.h"
#include "Export_Function.h"

BEGIN(Engine)

class ENGINE_DLL CZFrustum :
    public CBase
{
public:
	explicit CZFrustum();
	explicit CZFrustum(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CZFrustum(const CZFrustum& rhs);
	virtual ~CZFrustum();

public:

	BOOL	Ready_ZFrustum(_matrix* _pmatViewProj);
	BOOL	IsIn(_vec3& pv);
	BOOL	IsInSphere(_vec3& pv, _float radius);
	BOOL	Draw(LPDIRECT3DDEVICE9 pGraphicDev);
	_vec3*	GetPos() { return &m_vPos; }

private:
	_vec3		m_vtx[8];
	_vec3		m_vPos;
	D3DXPLANE	m_tPlane[6];
};

END