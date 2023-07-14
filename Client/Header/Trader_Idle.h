#pragma once
#include "State.h"
class CNpc_Trader_Idle :
    public CState
{
private:
	explicit CNpc_Trader_Idle();
	explicit CNpc_Trader_Idle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNpc_Trader_Idle();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:

public:
	static CNpc_Trader_Idle* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

