#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRigidBody;
class CCollider;
class CBasicStat;
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
    virtual void Init_Stat() PURE;

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
    _bool               Get_WallTouch()                      { return m_bWallTouch; }
    _vec3               Get_CenterPos()                      { return m_vCenterPos; }
    _bool               IsKnockBack()                        { return m_bKnockback; }
    _float              Get_MoveRange()                      { return m_fMoveReturnRange; }
    _float              Get_RandomoMoveRange()               { return m_fRandomMoveRange; }
    CRigidBody*         Get_RigidBody()                      { return m_pRigidBody; }
    _bool	            IsJump()                             { return m_bIsJump; }
    _float              Get_JumpVelocity()                   { return m_fJumpVelocity; }

public: // Setter
    void                Set_Speed(_float _fSpeed)            { m_fSpeed = _fSpeed; }
    void				Set_State(STATE _eState)             { m_eState = _eState; }
    void				Set_AttackTick(_bool _Tick)          { m_bAttackTick = _Tick; }
    void		        Set_MonsterState(MONSTERTAG _eState) { m_eMonsterTag = _eState; }

    void                Set_BlockOn(_bool _block)            { m_bBlockOn = _block; }
	void                Set_WallTouch(_bool _Touch)			 { m_bWallTouch = _Touch; }
    void                Set_CenterPos(_vec3 _vPos)           { m_vCenterPos = _vPos; }
    void                Set_KnockBack(_bool _KnockBack)      { m_bKnockback = _KnockBack; }
    void                Set_MoveRange(_float _fRange)        { m_fMoveReturnRange = _fRange; }
    void                Set_RandomMoveRange(_float _fRandom) { m_fRandomMoveRange = _fRandom; }
    void                Set_Jump(_bool _Jump)                { m_bIsJump = _Jump; }
    void                Set_JumpVelocity(_float _Velocity)   { m_fJumpVelocity = _Velocity; }
public:
    void                IsAttack(CBasicStat* _PlayerStat);
    void                Add_JumpVelocity(_float _Velocity)   { m_fJumpVelocity += _Velocity; }

protected:
    CRigidBody*         m_pRigidBody = nullptr;
    CStateMachine*      m_pStateMachine = nullptr;
    CBasicStat*         m_pBasicStat = nullptr;
    MONSTERTAG			m_eMonsterTag;
    STATE				m_eState;
    _bool				m_bAttackTick;
    _bool		        m_bBlockOn;
    _bool               m_bWallTouch;
    _bool               m_bKnockback;
    _bool		        m_bIsJump;
    _bool               m_bDeadCheck = false;

    _vec3               m_vCenterPos;
    _float              m_fMoveReturnRange;
    _float              m_fRandomMoveRange;
    _float              m_fJumpVelocity;

    _float              m_fSpeed;
 
    _bool               m_bDieEffect = false;
    _float              m_fDeadCoolTime;

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