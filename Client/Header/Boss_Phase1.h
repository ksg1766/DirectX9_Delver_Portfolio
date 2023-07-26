#pragma once
#include "State.h"
class CBoss_Phase1 :
    public CState
{
private: 
    explicit CBoss_Phase1();
    explicit CBoss_Phase1(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_Phase1();

public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //클래스 내에서만 쓸 함수
    STATE   BossSkill(const _float& fTimeDelta);
private:
    //변수
    _float  m_fDelay = 0;
    _uint   m_iSkillCount;
    _bool   m_bMeteor;
public:
    static CBoss_Phase1* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();

};

