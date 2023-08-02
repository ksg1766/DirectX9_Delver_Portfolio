#pragma once
#include "State.h"

class CTeleportPattern :
    public CState
{
private:
    explicit CTeleportPattern();
    explicit CTeleportPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CTeleportPattern();


public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //클래스 내에서만 쓸 함수
    void    Make_BatSwarm(const _float& fTimeDelta);
private:
    //클래스 내에서만 쓸 함수
    _vec3   m_vDir;
    _vec3   m_fBatMovePos[8];
    _float  m_fSpawnHeight;
    _float  m_fSpawnWeight;
    _float m_fDelay;
    _float  m_bLastDelay;
    _bool    m_bSkillCount;
    _bool   m_bSound;
public:
    static CTeleportPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};



