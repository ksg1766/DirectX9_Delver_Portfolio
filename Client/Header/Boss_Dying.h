#pragma once
#include "State.h"

class CBoss_Dying :
    public CState
{
private:
    explicit CBoss_Dying();
    explicit CBoss_Dying(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_Dying();


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
    _float          m_fDelay;
    _bool           m_bSound;
public:
    static CBoss_Dying* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

