#pragma once
#include "State.h"

class CBlade_Trap_Idle :
    public CState
{
private:
    explicit CBlade_Trap_Idle();
    explicit CBlade_Trap_Idle(LPDIRECT3DDEVICE9 pGrapicDev);
    virtual ~CBlade_Trap_Idle();

public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //클래스 내에서만 쓸 함수
    //STATE 
private:
    //변수

public:
    static CBlade_Trap_Idle* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

