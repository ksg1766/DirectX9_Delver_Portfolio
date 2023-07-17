#pragma once

#include "State.h"

class CGhost_Attack  : public CState
{
private:
	explicit CGhost_Attack();
	explicit CGhost_Attack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGhost_Attack();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }


private:
	_bool	m_bIsAttack;
	_int	m_iCount;

public:
	static CGhost_Attack* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

