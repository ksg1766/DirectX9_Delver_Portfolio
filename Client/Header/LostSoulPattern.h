#pragma once
#include "State.h"
class CLostSoulPattern :
    public CState
{
private:
    explicit CLostSoulPattern();
    explicit CLostSoulPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CLostSoulPattern();

public:
    virtual HRESULT	    Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
public:
    static CLostSoulPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);
private:
    //클래스 내에서만 쓸 함수
private:
    //클래스 내에서만 쓸 함수
    _float m_fDelay;
    _float  m_fPatternDelay;
    _int    m_iSkillCount;
private:
    virtual void Free();
};

