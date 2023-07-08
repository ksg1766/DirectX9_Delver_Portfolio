#pragma once

#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;
class CBossAI;
END
class CTerrain;

class CSkeletonKing:public Engine::CGameObject
{
private:
	explicit	CSkeletonKing(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CSkeletonKing(const CSkeletonKing& rhs);
	virtual		~CSkeletonKing();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

public:
	void		ForceHeight(_vec3 _vPos);
	void		Set_Terrain(CTerrain* _pCurrentTerrain) { m_pTerrain = _pCurrentTerrain; }

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);

private:
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture = nullptr;
	CBillBoard*	m_pBillBoard = nullptr;
	CBossAI*	m_pBossAI = nullptr;
	CTerrain*	m_pTerrain = nullptr;
	_float		m_fFrame = 0;

public:
	static CSkeletonKing* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

