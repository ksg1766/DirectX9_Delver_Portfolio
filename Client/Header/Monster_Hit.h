#pragma once

#include "State.h"

BEGIN(Engine)

END

class CMonster_Hit : public CState
{
private:
	explicit CMonster_Hit();
	explicit CMonster_Hit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster_Hit();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
	void	Hit_Sound();

private:
	_bool	m_bCanHitState; 
	_float  m_fHitCoolDown;

	_int	m_iHitCount;

public:
	static CMonster_Hit* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};