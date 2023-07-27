#pragma once

#include "State.h"

BEGIN(Engine)

END

class CMonster_Dead : public CState
{
private:
	explicit CMonster_Dead();
	explicit CMonster_Dead(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster_Dead();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
	void Monster_Dead();


private:
	_float	m_fFlyDown;
	_bool	m_bDead = false;

public:
	static CMonster_Dead* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};