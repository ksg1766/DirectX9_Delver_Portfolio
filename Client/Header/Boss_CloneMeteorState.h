#pragma once
#include "State.h"
class CBoss_CloneMeteorState :
    public CState
{
private:
    explicit CBoss_CloneMeteorState();
    explicit CBoss_CloneMeteorState(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_CloneMeteorState();

public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();
public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

public:
    static CBoss_CloneMeteorState* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);
private:
    _float  m_fDelay;
private:
    virtual void Free();
};

