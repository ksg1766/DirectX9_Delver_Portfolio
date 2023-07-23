#pragma once
#include "State.h"
class CCrossPattern :
    public CState
{
private:
    explicit CCrossPattern();
    explicit CCrossPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CCrossPattern();


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
    static CCrossPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};
