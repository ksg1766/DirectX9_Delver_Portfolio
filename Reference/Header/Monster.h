#pragma once

#include "GameObject.h"

class CTerrain;

BEGIN(Engine)

class CCollider;
class ENGINE_DLL CMonster : public CGameObject
{
protected:
    explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMonster(const CMonster& rhs);
    virtual ~CMonster();

public:
    HRESULT     Ready_Object();
    _int        Update_Object(const _float& fTimeDelta);

public:
    virtual void		OnCollisionEnter(CCollider* _pOther);
    virtual void		OnCollisionStay(CCollider* _pOther);
    virtual void		OnCollisionExit(CCollider* _pOther);

public: //Getter
    _float              Get_Speed()                          { return m_fSpeed; }
    CStateMachine*      Get_StateMachine()                   { return m_pStateMachine; }
    CBasicStat*         Get_BasicStat()                      { return m_pBasicStat; }
    MONSTERTAG	        Get_MonsterTag()                     { return m_eMonsterTag; }
    STATE				Get_State()                          { return m_eState; }
    _bool				Get_AttackTick()                     { return m_bAttackTick; }
    _bool               Get_BlockOn()                        { return m_bBlockOn; }

public: // Stter
    void                Set_Speed(_float _fSpeed)            { m_fSpeed = _fSpeed; }
    void				Set_State(STATE _eState)             { m_eState = _eState; }
    void				Set_AttackTick(_bool _Tick)          { m_bAttackTick = _Tick; }
    void		        Set_MonsterState(MONSTERTAG _eState) { m_eMonsterTag = _eState; }

    void                Set_BlockOn(_bool _block)            { m_bBlockOn = _block; }
    void                Set_Terrain(CTerrain* _pTerrain)     { m_pTerrain = _pTerrain; }

protected:
    CStateMachine*                      m_pStateMachine = nullptr;
    CBasicStat*                         m_pBasicStat = nullptr;
    MONSTERTAG							m_eMonsterTag;
    STATE								m_eState;
    _bool								m_bAttackTick;
    _bool		                        m_bBlockOn;
    _float                              m_fSpeed;
    CTerrain*                           m_pTerrain;
    // MONSTER_ID m_eMonserID;
    // MonsterAI Component 추가해서 사용

// CGameObject로부터 상속
    //map<COMPONENTTAG, CComponent*>	m_mapComponent[ID_END];
    //LPDIRECT3DDEVICE9				  m_pGraphicDev;
    //_bool								m_IsDead;

   // OBJECTTAG						  m_eObjectTag;
    //CTransform*                       m_pTransform;	// Transform Attached to this GameObject

    

protected:
    virtual void	Free(void);
};

END