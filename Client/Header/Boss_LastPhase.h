#pragma once
#include "State.h"
class CBoss_LastPhase :
    public CState
{
private:
    explicit CBoss_LastPhase();
    explicit CBoss_LastPhase(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBoss_LastPhase();

public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    _float m_fDelay;
    _float m_fHekiRekiDelay;
    _vec3   m_vDir;
    _bool   m_bSound;
public:
    static CBoss_LastPhase* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

