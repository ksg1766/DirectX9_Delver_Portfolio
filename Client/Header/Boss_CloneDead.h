#pragma once
#include "State.h"
class CBoss_CloneDead :
    public CState
{
private:
    explicit CBoss_CloneDead();
    explicit CBoss_CloneDead(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_CloneDead();
public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();
public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

public:
    static CBoss_CloneDead* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);
private:
    _float  m_fDeadDelay;
private:
    virtual void Free();
};

