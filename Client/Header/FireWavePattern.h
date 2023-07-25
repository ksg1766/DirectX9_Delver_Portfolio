#pragma once
#include "State.h"
class CFireWavePattern :
    public CState
{
private:
    explicit CFireWavePattern();
    explicit CFireWavePattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CFireWavePattern();


public:
    virtual HRESULT	    Ready_State(CStateMachine* pOwner);
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
    _int    m_iCount;
    _bool  m_bCool;
    _bool   m_bWarning;
    _vec3 m_vWavePos;
public:
    static CFireWavePattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

