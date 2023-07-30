#pragma once
#include "State.h"
class CLostSoul_State :
    public CState
{
private:
    explicit CLostSoul_State();
    explicit CLostSoul_State(LPDIRECT3DDEVICE9 pGrapicDev);
    virtual ~CLostSoul_State();
public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:

private:

public:
    static CLostSoul_State* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

