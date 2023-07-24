#pragma once
#include "State.h"
class CMeteorPh3 :
    public CState
{
private:
    explicit CMeteorPh3();
    explicit CMeteorPh3(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CMeteorPh3();


public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
    void    Reset_Member();
private:
    void    Make_LostSoul();
private:
    _float  m_fDelay;
    _float  m_fSpawnDelay;
    _float  m_fPatternDelay;
    _float m_fChannel_Count;
    _bool   m_bSkill;
    _bool   m_bSkillStart;
    _vec3   m_vSpawnPos[3];
public:
    static CMeteorPh3* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();

};

