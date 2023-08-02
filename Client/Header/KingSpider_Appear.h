#pragma once
#include "State.h"
class CKingSpider_Appear :
    public CState
{
private:
	explicit CKingSpider_Appear();
	explicit CKingSpider_Appear(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKingSpider_Appear();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();
public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
	_bool			Get_JumpTrigger() { return m_bJumpTrigger; }
private:
	_bool	m_bAppearTrigger;
	_bool	m_bJumpTrigger;
	_bool	m_bLanding;
	_bool	m_bSound;
	_float	m_fMoveDelay;
	_float	m_fFrameDelay;
	_float	m_fDelay;
	_vec3	m_vDir;
	_vec3	m_vTargetPos;
	_vec3	m_vFogPos[8];
public:
	static CKingSpider_Appear* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

