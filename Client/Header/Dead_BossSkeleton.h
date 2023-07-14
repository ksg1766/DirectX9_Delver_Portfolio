#pragma once
#include "State.h"
class CDead_BossSkeleton :
    public CState
{
private:
    explicit CDead_BossSkeleton();
    explicit CDead_BossSkeleton(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CDead_BossSkeleton();


public:
    virtual HRESULT    Ready_State(CStateMachine* pOwner);
    virtual STATE        Update_State(const _float& fTimeDelta);
    virtual  void        LateUpdate_State();
    virtual  void        Render_State();

public:
    virtual STATE    Key_Input(const _float& fTimeDelta) { return STATE(); }
private:

private:
    
public:
    static CDead_BossSkeleton* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

