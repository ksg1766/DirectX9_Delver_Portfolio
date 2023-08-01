#pragma once
#include "State.h"
class CPhantom_Idle :
	public CState
{
private:
	explicit CPhantom_Idle();
	explicit CPhantom_Idle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPhantom_Idle();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:

public:
	static CPhantom_Idle* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

