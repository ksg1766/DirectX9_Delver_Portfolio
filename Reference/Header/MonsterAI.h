#pragma once
#include "Component.h"

BEGIN(Engine)

class CStateMachine;

class ENGINE_DLL CMonsterAI :
    public CComponent
{
protected:
	explicit CMonsterAI();
	explicit CMonsterAI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterAI(const CMonsterAI& rhs);
	virtual ~CMonsterAI();

public:
	void			Init_Property(_float _fSight, _float _fAttack, _vec3 _vRoamingCen, _float _RoamingRad, _float _fMaxTrace);
	
	// 트랜스폼 세팅 해야함.
	//void			Set_AITransform(CTransform* _pTransform) { m_pTransform = _pTransform; }


	// 플레이어 추적류
	const _matrix* Compute_LookAtTarget(const _vec3* pTargetPos);
	void	Chase_Target(const _vec3* pTargetPos, const _float& fTimeDelta, const _float& fSpeed);

public: // Getter
	CStateMachine*	Get_StateMachine() { return m_pStateMachine; }

public: // Setter
	void		Set_StateMachine(CStateMachine* _pStateMachine) { m_pStateMachine = _pStateMachine; }

protected:
	void			Auto_Roaming();
	void			Auto_Attack();
	void			Back_Home();

	_bool			Out_Of_Town();

	

public:
	HRESULT			Ready_MonsterAI();
	virtual _int	Update_Component(const _float& fTimeDelta);

protected:
	_float	m_fAttackRange;

	_vec3	m_vRoamingCenter;
	_float	m_fRoamingRadius;

	_float	m_fMaxTraceRadius;

	// 내가 쓸 변수들
	_vec3	m_vDistance;
	_float	m_fSightRange;
	_float  m_fDistance;
	_float	m_fSpeed;
	_float	m_fChase;
	_float  m_fJumpInitializeVelocity;


	_bool  m_bIsJumping;
	STATE  m_eState;

	CStateMachine* m_pStateMachine;

public:
	static	CMonsterAI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);

protected:
	virtual void			Free();

};

END