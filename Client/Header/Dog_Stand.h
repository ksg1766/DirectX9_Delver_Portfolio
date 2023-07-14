#pragma once
#include "State.h"
class CNpc_Dog_Stand :
    public CState
{
private:
	explicit CNpc_Dog_Stand();
	explicit CNpc_Dog_Stand(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNpc_Dog_Stand();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:

public:
	static CNpc_Dog_Stand* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

