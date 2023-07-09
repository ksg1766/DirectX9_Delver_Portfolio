#pragma once
#include "State.h"
class CBoss_Attack :
    public CState
{
private: 
    explicit CBoss_Attack();
    explicit CBoss_Attack(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_Attack();

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
    void    BossTeleport();
    void    FireShogun(const _float& fTimeDelta);
    void    Explosion_Flower(const _float& fTimeDelta);
private:
    //변수
    _int iTime = 0;
    _vec3 m_vShotGunDir[3];

    _bool   m_bExplosion;
    _vec3 m_vExplosionin1[4];
    _vec3 m_vExplosionin2[4];

public:
    static CBoss_Attack* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();

};

