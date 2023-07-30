#pragma once
#include "State.h"

class CBoss_MeteorCube;

class CBoss_Meteor2Ph :
    public CState
{
private:
    explicit CBoss_Meteor2Ph();
    explicit CBoss_Meteor2Ph(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_Meteor2Ph();


public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //클래스 내에서만 쓸 함수
    void    Make_Clone();
    void    Make_LostSoul();
    void    Move_DIr();
private:
    //클래스 내에서만 쓸 함수
    _float m_fDelay;
    _float  m_fPatternDelay;
    _bool  m_bSkill;
    _bool   m_bMove;
    _bool   m_bSkillStart;
    _bool   m_bPosReset;
    _float m_fChannel_Count;
    _vec3   m_vPillarPos[3];
    _vec3   m_vTargetPos;
    _vec3   m_vDir;
    _int    m_iIndex;
public:
    static CBoss_Meteor2Ph* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

