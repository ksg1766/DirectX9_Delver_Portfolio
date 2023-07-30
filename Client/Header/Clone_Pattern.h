#pragma once
#include "State.h"
class CClone_Pattern :
    public CState
{
private:
    explicit CClone_Pattern();
    explicit CClone_Pattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CClone_Pattern();


public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

    STATE Move_DIr();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //클래스 내에서만 쓸 함수
private:
    //클래스 내에서만 쓸 함수
    _float m_fDelay;
    _float m_fMoveDelay;
    _bool  m_bSkill;
    _bool   m_bPosReset;
    _bool   m_bSound;
    _vec3   m_vPillarPos[3];
    _vec3   m_vTargetPos;
    _vec3   m_vResetPos;
    _vec3   m_vDir;
    _uint   m_iIndex;
public:
    static CClone_Pattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

