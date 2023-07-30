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

class CSkeletonKing_Clone;
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
	_bool			Get_Sturn() { return m_bSturn; }
	_bool			Get_3Phase() { return m_b3Phase; }
	_uint			Get_CloneCount() { return m_iCloneCount; }
	BOSSPHASE		Get_Phase() { return m_ePhase; }
	vector<CSkeletonKing_Clone*>	Get_CloneVector() { return m_vecSkeletonClone; }
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);
	void			Set_Sturn(_bool _sturn) { m_bSturn = _sturn; }
	void			Set_CloneCount(_int _Clone) { m_iCloneCount = _Clone; }
	void			Set_Phase(BOSSPHASE _PHASE) { m_ePhase = _PHASE; }
	void			Set_3Phase(_bool _3phase) { m_b3Phase = _3phase; }

	void			Add_CloneCount(_int _Hit);
	void			Add_HitCount();

	void			Set_Dir(_vec3 _vDir);
	void			MoveToDir(const _float& fTimeDelta);

private:
	HRESULT		Add_Component(void);
	void		Change_Phase();

private:
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture[(_uint)STATE::STATE_END] = {};
	CBillBoard*	m_pBillBoard = nullptr;
	CAnimator*  m_pAnimator = nullptr;
	CState*		m_pState = nullptr;
	_float		m_fFrame = 0;
	_float		m_fHitCool;
	_float		m_fMoveDelay;
	_float		m_fDelay;
	_int		m_iHitCount;
	_bool		m_bSturn;
	_bool		m_b3Phase;
	_bool		m_bMove;
	_uint		m_iCloneCount;
	_vec3		m_vTargetPos;
	_vec3		m_vDir;
	BOSSPHASE	m_ePhase;
	vector<CSkeletonKing_Clone*>	m_vecSkeletonClone;

public:
	static CSkeletonKing* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

