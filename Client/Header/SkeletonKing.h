#pragma once

#include "Monster.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;
class CStateMachine;
class CAnimator;

END

class CSkeletonKing:public Engine::CMonster
{
private:
	explicit	CSkeletonKing(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CSkeletonKing(const CSkeletonKing& rhs);
	virtual		~CSkeletonKing();

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int Update_Object(const _float& fTimeDelta)	override;
	virtual void LateUpdate_Object(void)					override;
	virtual void Render_Object(void)						override;

public:
	virtual	void	Init_Stat()								override;

	void			ReSet_Sturn() { m_iHitCount = 0; }
	virtual _float	Get_Frame() { return m_fFrame; }
	CStateMachine*	Get_StatMachine() { return m_pStateMachine; }
	_bool			Get_Sturn() { return m_bSturn; }

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);
	void			Set_Sturn(_bool _sturn) { m_bSturn = _sturn; }
	void			Add_HitCount();
private:
	HRESULT		Add_Component(void);
	void		Key_Input();

private:
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture[(_uint)STATE::STATE_END] = {};
	CBillBoard*	m_pBillBoard = nullptr;
	CStateMachine* m_pStateMachine = nullptr;

	CAnimator*  m_pAnimator = nullptr;

	_float		m_fFrame = 0;
	_float		m_fHitCool;
	_uint		m_iHitCount;
	_bool		m_bSturn;
public:
	static CSkeletonKing* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

