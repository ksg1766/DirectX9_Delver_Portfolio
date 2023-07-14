#pragma once
#include "State.h"
class CExplosion_BossSkeleton :
    public CState
{
private:
    explicit CExplosion_BossSkeleton();
    explicit CExplosion_BossSkeleton(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CExplosion_BossSkeleton();


public:
    virtual HRESULT    Ready_State(CStateMachine* pOwner);
    virtual STATE        Update_State(const _float& fTimeDelta);
    virtual  void        LateUpdate_State();
    virtual  void        Render_State();

public:
    virtual STATE    Key_Input(const _float& fTimeDelta) { return STATE(); }
private:

private:
    _bool   m_bExplosion;
public:
    static CExplosion_BossSkeleton* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

