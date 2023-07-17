#pragma once

#include "State.h"

class CGhost_IDLE : public CState
{
private:
	explicit CGhost_IDLE();
	explicit CGhost_IDLE(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGhost_IDLE();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }


private:

public:
	static CGhost_IDLE* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

