#pragma once

#include "State.h"

BEGIN(Engine)

END

class CLongitudinal : public CState
{
private:
	explicit CLongitudinal();
	explicit CLongitudinal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLongitudinal();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

public:
	static CLongitudinal* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:

private:
	virtual void Free();
};

