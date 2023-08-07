#pragma once
#include "State.h"

class CBoss_WakeUp :
    public CState
{
private:
    explicit CBoss_WakeUp();
    explicit CBoss_WakeUp(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_WakeUp();


public:
    virtual HRESULT 	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual	void		LateUpdate_State();
    virtual void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //클래스 내에서만 쓸 함수
    //STATE 
private:
    //변수
    _float  m_fDelay;
    _float  m_fTest;
    _bool   m_bWakeUp;

    _bool   m_bTitle = true;

public:
    static CBoss_WakeUp* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

