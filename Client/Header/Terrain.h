#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CTransform;

END

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

public:
	vector<_vec3>& LoadTerrainVertex() { return m_vecTerrainVertex; }

private:
	HRESULT		Add_Component(void);

private:
	CTerrainTex*	m_pBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;

	vector<_vec3>	m_vecTerrainVertex;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

// 1. 평면의 방정식을 이용하여 플레이어를 지형에 타게 만들어라

// D3DXPLANE Plane;
// D3DXPlaneFromPoints()
// ax + by + cz + d