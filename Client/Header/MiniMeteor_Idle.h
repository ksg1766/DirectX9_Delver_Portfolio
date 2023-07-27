#pragma once
#include "State.h"
class CMiniMeteor_Idle :
    public CState
{
private:
    explicit CMiniMeteor_Idle();
    explicit CMiniMeteor_Idle(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CMiniMeteor_Idle();
public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();
public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

public:
    static CMiniMeteor_Idle* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);
private:
    _float  m_fDelay;
private:
    virtual void Free();
};

