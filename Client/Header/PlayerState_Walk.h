#pragma once
#include "State.h"

BEGIN(Engine)

END

class CPlayerState_Walk : public CState
{
private:
	explicit CPlayerState_Walk();
	explicit CPlayerState_Walk(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerState_Walk();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta);


public:
	static CPlayerState_Walk* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);
private:
	virtual void Free();
};

