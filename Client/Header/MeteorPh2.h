#pragma once
#include "State.h"
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
    //Ŭ���� �������� �� �Լ�
private:
    //Ŭ���� �������� �� �Լ�
    _float m_fDelay;
    _float  m_fPatternDelay;
    _bool  m_bCool;
public:
    static CBoss_Meteor2Ph* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};
