#pragma once
#include "State.h"
class CBoss_SkeletonSpawnPattern :
    public CState
{
private:
    explicit CBoss_SkeletonSpawnPattern();
    explicit CBoss_SkeletonSpawnPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_SkeletonSpawnPattern();

public:
    virtual HRESULT    Ready_State(CStateMachine* pOwner);
    virtual STATE        Update_State(const _float& fTimeDelta);
    virtual  void        LateUpdate_State();
    virtual  void        Render_State();

public:
    virtual STATE    Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    void    Spawn_Skeleton();
private:

    _bool   m_bSkill;
    _vec3 m_vSpawnPos1[2];
    _vec3 m_vSpawnPos2[2];
    _float  m_fSkillCool;
public:
    static CBoss_SkeletonSpawnPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

