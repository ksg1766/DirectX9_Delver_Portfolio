#pragma once
#include "State.h"
class CBoss_CloneMeteorDead :
    public CState
{
private:
    explicit CBoss_CloneMeteorDead();
    explicit CBoss_CloneMeteorDead(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_CloneMeteorDead();
public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();
public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

public:
    static CBoss_CloneMeteorDead* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);
private:
    _float  m_fDeadDelay;
private:
    virtual void Free();
};

