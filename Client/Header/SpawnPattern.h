#pragma once
#include "State.h"

class CSpawnPattern :
    public CState
{
private:
    explicit CSpawnPattern();
    explicit CSpawnPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CSpawnPattern();

public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
    
private:
    //클래스 내에서만 쓸 함수
    void    Spawn_Skeleton();
private:
    //클래스 내에서만 쓸 함수
    _bool   m_bSkill;
    _vec3 m_vSpawnPos[4];
    _float  m_fSkillCool;
public:
    static CSpawnPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

