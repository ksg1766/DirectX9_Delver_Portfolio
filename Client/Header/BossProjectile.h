#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;
END
class CTerrain;

class CBossProjectile :
    public CGameObject
{
private:
	explicit	CBossProjectile(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBossProjectile(const CBossProjectile& rhs);
	virtual		~CBossProjectile();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

public:
	float		HitTerrain(_vec3 _vPos);

	//테스트용충돌코드
	void		Set_Terrain(CTerrain* _pCurrentTerrain) { m_pTerrain = _pCurrentTerrain; }
	void		Set_Target(_vec3 _vPos);
private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;
	CTerrain* m_pTerrain = nullptr;
	_float			m_fFrame;
	_vec3 m_vTargetPos;
	_vec3 m_vDir;
private:
	HRESULT		Add_Component(void);

public:
	static CBossProjectile* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

