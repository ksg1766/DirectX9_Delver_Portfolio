#pragma once

#include "Monster.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;
class CStateMachine;
class CAnimator;

END
class CTerrain;

class CSkeletonKing:public Engine::CMonster
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
	CTerrain*	Get_Terrain() { return m_pTerrain; }
	virtual _float		Get_Frame() { return m_fFrame; }
	CStateMachine* Get_StatMachine() { return m_pStateMachine; }

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	HRESULT		Add_Component(void);
	void		Key_Input();
private:
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture[(_uint)STATE::STATE_END] = {};
	CBillBoard*	m_pBillBoard = nullptr;
	CTerrain*	m_pTerrain = nullptr;
	CStateMachine* m_pStateMachine = nullptr;

	CAnimator*  m_pAnimator = nullptr;

	_float		m_fFrame = 0;

public:
	static CSkeletonKing* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

