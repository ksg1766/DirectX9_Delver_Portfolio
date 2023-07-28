#pragma once
#include "State.h"
class CCrawlPattern :
    public CState
{
private:
    explicit CCrawlPattern();
    explicit CCrawlPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CCrawlPattern();


public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //클래스 내에서만 쓸 함수
    void    Dash(const _float& fTimeDelta);
private:
    //클래스 내에서만 쓸 함수
    _bool   m_bTarget;
    _vec3 m_vTargetPos;
    _vec3 m_vDir;

    _float m_fDelay;
    _float m_fSpeed;
    _int    m_iSkillCount;
    
public:
    static CCrawlPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();

};

