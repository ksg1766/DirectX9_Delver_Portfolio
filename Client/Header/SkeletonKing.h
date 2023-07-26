#pragma once
#include "Monster.h"
#include "Engine_Enum.h"
BEGIN(Engine)
class CRcTex;
class CTexture;
class CBillBoard;
class CStateMachine;
class CAnimator;
class CState;
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
	_uint			Get_CloneCount() { return m_iCloneCount; }
	BOSSPHASE		Get_Phase() { return m_ePhase; }

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);
	void			Set_Sturn(_bool _sturn) { m_bSturn = _sturn; }
	void			Set_CloneCount(_int _Clone) { m_iCloneCount = _Clone; }
	void			Set_Phase(BOSSPHASE _PHASE) { m_ePhase = _PHASE; }

	void			Add_CloneCount(_int _Hit);
	void			Add_HitCount();
private:
	HRESULT		Add_Component(void);
	void		Change_Phase();
	void		Key_Input();

private:
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture[(_uint)STATE::STATE_END] = {};
	CBillBoard*	m_pBillBoard = nullptr;
	CAnimator*  m_pAnimator = nullptr;
	CState*		 m_pState = nullptr;
	_float		m_fFrame = 0;
	_float		m_fHitCool;
	_int		m_iHitCount;
	_bool		m_bSturn;
	_uint		m_iCloneCount;
	BOSSPHASE	m_ePhase;
public:
	static CSkeletonKing* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

