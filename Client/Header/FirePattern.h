#pragma once
#include "State.h"
class CFirePattern :
    public CState
{
private:
    explicit CFirePattern();
    explicit CFirePattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CFirePattern();


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
    _int    m_iSkillCount;
public:
    static CFirePattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

