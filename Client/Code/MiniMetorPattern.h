#pragma once
#include "State.h"
class CMiniMetorPattern :
    public CState
{
private:
    explicit CMiniMetorPattern();
    explicit CMiniMetorPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CMiniMetorPattern();

public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();
public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //클래스 내에서만 쓸 함수
private:
    //클래스 내에서만 쓸 함수
public:
    static CMiniMetorPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

