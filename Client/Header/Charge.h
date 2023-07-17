#pragma once

#include "State.h"

class CCharge : public CState
{
private:
	explicit CCharge();
	explicit CCharge(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCharge();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
	_vec3	m_vPrevPos;
	_bool	m_bIsAttack;
	_float	m_fSpeed;
	_float	m_CoolTime;


	_bool	m_bAttackTick;
	_bool	m_bAttackTick2;
	_bool	m_bCheck;

private:

public:
	static CCharge* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

