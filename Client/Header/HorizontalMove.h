#pragma once

#include "Stage.h"



class CHorizontalMove : public CState
{
private:
	explicit CHorizontalMove();
	explicit CHorizontalMove(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHorizontalMove();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

public:
	static CHorizontalMove* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	_float	m_fAttackCool;


private:
	virtual void Free();
};

