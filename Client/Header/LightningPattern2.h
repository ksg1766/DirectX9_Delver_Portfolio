#pragma once
#include "State.h"
class CLightningPattern2 :
    public CState
{
private:
    explicit CLightningPattern2();
    explicit CLightningPattern2(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CLightningPattern2();


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
    _float m_fDelay;
    _float  m_fPatternDelay;
    _bool  m_bCool;

    _vec3   m_vPlayerPos;
    _vec3   m_vCrossDir[4];
    _float  m_fDuration;
    _float  m_fRandomX;
    _float  m_fRandomZ;
public:
    static CLightningPattern2* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

