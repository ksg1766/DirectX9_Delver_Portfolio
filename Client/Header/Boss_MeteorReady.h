#pragma once
#include "State.h"

class CBoss_MeteorCube;

class CBoss_MeteorReady :
    public CState
{
private:
    explicit CBoss_MeteorReady();
    explicit CBoss_MeteorReady(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_MeteorReady();

public:
    virtual HRESULT 	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual	void		LateUpdate_State();
    virtual void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //클래스 내에서만 쓸 함수

private:
    //변수
    CBoss_MeteorCube* m_pGameObject = nullptr;
    _float m_fChannel_Count;
    _bool   m_bSkillStart;
public:
    static CBoss_MeteorReady* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

