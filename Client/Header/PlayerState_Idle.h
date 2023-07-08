#pragma once
#include "State.h"

BEGIN(Engine)

END

class CPlayerState_Idle : public CState
{
private:
	explicit CPlayerState_Idle();
	explicit CPlayerState_Idle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerState_Idle();

public:
	virtual HRESULT Ready_State(CStateMachine* pOwner);
	virtual STATE Update_State(const _float& fTimeDelta);
	virtual void  LateUpdate_State();
	virtual void  Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta);

public:
	static CPlayerState_Idle* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

