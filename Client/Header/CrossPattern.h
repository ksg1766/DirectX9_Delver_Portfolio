#pragma once
#include "State.h"
class CCrossPattern :
    public CState
{
private:
    explicit CCrossPattern();
    explicit CCrossPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CCrossPattern();


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
    _float m_fCautionDelay;
    _float  m_fPatternDelay;
    _bool  m_bCool;

    _vec3   m_vHeight;
    _vec3   m_vCrossDir[4];
    _int    m_iSkillCount;
    _int    m_iCautionCount;
public:
    static CCrossPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

