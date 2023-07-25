#pragma once
#include "State.h"

BEGIN(Engine)

END

class CMonster_Jump : public CState
{
private:
	explicit CMonster_Jump();
	explicit CMonster_Jump(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster_Jump();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
	STATE Jump(const _float& fTimeDelta);
		
private:
	_vec3	m_vLastPos;
	_vec3	m_vSavePos;


	_bool	m_bIsJumping;
	_bool	m_bStartJumpPos;
	_float	m_fJumpVelocity;

	_bool	m_bJumCoolDown;
	_float	m_fJumpCoolDuration;
	_float	m_fJumpCoolTimer;
public:
	static CMonster_Jump* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

