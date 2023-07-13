#pragma once
#include "State.h"
#pragma once
#include "State.h"
class CMove_BossSkeleton :
    public CState
{
private:
    explicit CMove_BossSkeleton();
    explicit CMove_BossSkeleton(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CMove_BossSkeleton();


public:
    virtual HRESULT    Ready_State(CStateMachine* pOwner);
    virtual STATE        Update_State(const _float& fTimeDelta);
    virtual  void        LateUpdate_State();
    virtual  void        Render_State();

public:
    virtual STATE    Key_Input(const _float& fTimeDelta) { return STATE(); }
private:

private:
    _vec3   m_vTargetPos;
    _vec3   m_vDir;
    _float  m_fSpeed;
    _float  m_fCool;
public:
    static CMove_BossSkeleton* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

