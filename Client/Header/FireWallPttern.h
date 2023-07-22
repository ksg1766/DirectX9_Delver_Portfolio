#pragma once
#include "State.h"

class CFireWallPttern :
    public CState
{
private:
    explicit CFireWallPttern();
    explicit CFireWallPttern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CFireWallPttern();

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
    _vec3   m_vCenterDistance[4];
    _bool   m_bCool;
    _float  m_fDuration;
public:
    static CFireWallPttern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

