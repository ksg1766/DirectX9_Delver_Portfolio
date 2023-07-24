#pragma once
#include "State.h"
class CLostSoulPattern2 :
    public CState
{
private:
    explicit CLostSoulPattern2();
    explicit CLostSoulPattern2(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CLostSoulPattern2();

public:
    virtual HRESULT	    Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
public:
    static CLostSoulPattern2* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);
private:
    //클래스 내에서만 쓸 함수
    _float m_fDelay;
    _float  m_fPatternDelay;
    _int    m_iSkillCount;
    _vec3   m_vLaunchPos[5];
private:
    virtual void Free();
};
