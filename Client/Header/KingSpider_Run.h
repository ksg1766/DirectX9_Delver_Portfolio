#pragma once
#include "State.h"
class CKingSpider_Run :
    public CState
{
private:
	explicit CKingSpider_Run();
	explicit CKingSpider_Run(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKingSpider_Run();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();
public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

	_bool			Get_JumpCheck() { return	m_bJumpCheck; }
private:
	_bool	m_bRunTrigger;
	_bool	m_bJumpTrigger;
	_bool	m_bJumpCheck;
	_float	m_fMoveDelay;
	_float	m_fFrameDelay;
	_float	m_fDelay;
	_vec3	m_vDir;
	_vec3	m_vTargetPos;

public:
	static CKingSpider_Run* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

