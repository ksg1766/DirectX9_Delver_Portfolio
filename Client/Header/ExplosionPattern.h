#pragma once
#include "State.h"
class CExplosionPattern :
    public CState
{
private:
    explicit CExplosionPattern();
    explicit CExplosionPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CExplosionPattern();


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

    _vec3 m_vExplosionin1[4];
    _vec3 m_vExplosionin2[4];
public:
    static CExplosionPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

