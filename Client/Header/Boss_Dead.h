#pragma once
#include "State.h"

class CBoss_Dead :
    public CState
{
private:
    explicit CBoss_Dead();
    explicit CBoss_Dead(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_Dead();


public:
    virtual HRESULT 	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual	void		LateUpdate_State();
    virtual void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //클래스 내에서만 쓸 함수
private:
    //변수

public:
    static CBoss_Dead* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

