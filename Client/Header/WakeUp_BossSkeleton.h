#pragma once
#include "State.h"
#pragma once
#include "State.h"
class CWakeUp_BossSkeleton :
    public CState
{
private:
    explicit CWakeUp_BossSkeleton();
    explicit CWakeUp_BossSkeleton(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CWakeUp_BossSkeleton();


public:
    virtual HRESULT    Ready_State(CStateMachine* pOwner);
    virtual STATE        Update_State(const _float& fTimeDelta);
    virtual  void        LateUpdate_State();
    virtual  void        Render_State();

public:
    virtual STATE    Key_Input(const _float& fTimeDelta) { return STATE(); }
private:

private:
    _float m_fSpeed;
public:
    static CWakeUp_BossSkeleton* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

