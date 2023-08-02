#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCollider;

class ENGINE_DLL CNpc : public CGameObject
{
protected:
    explicit CNpc(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CNpc(const CNpc& rhs);
    virtual  ~CNpc();

public:
    HRESULT     Ready_Object();
    _int        Update_Object(const _float& fTimeDelta);

public:
    virtual void		OnCollisionEnter(CCollider* _pOther);
    virtual void		OnCollisionStay(CCollider* _pOther);
    virtual void		OnCollisionExit(CCollider* _pOther);


public:
    void				Set_State(STATE _eState) { m_eState = _eState; }
    CBasicStat*         Get_BasicStat() { return m_pBasicStat; }
    NPCTAG	            Get_NPCTag() { return m_eNPCTag; }
    STATE	            Get_State() { return m_eState; }

    _uint		        Get_SpeechCount() { return m_iCount; }
   // _bool				Get_AttackTick() { return m_bAttackTick; }

protected:
    CStateMachine*  m_pStateMachine = nullptr;
    CBasicStat*     m_pBasicStat = nullptr;
    NPCTAG		    m_eNPCTag;
    STATE			m_eState;

    _uint			m_iCount;
    _uint			m_iMaxCount;

    _bool			m_bAttackTick;
    //NPC를 때릴 수 있도록 일단 만들어봄

protected:
    virtual void	Free(void);
};
END

