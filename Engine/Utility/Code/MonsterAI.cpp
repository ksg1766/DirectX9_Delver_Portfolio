#include "..\..\Header\MonsterAI.h"
#include "Transform.h"
#include "GameObject.h"


CMonsterAI::CMonsterAI()
	:m_fSightRange(0.f)
	, m_fAttackRange(0.f)
	, m_vRoamingCenter(_vec3(0.f, 0.f, 0.f))
	, m_fRoamingRadius(0.f)
	, m_fMaxTraceRadius(0.f)
	, m_pStateMachine(nullptr)
	, m_fDistance(0.f)
	, m_vDistance(_vec3(0.f,0.f,0.f))
	, m_fSpeed(0.f)
	, m_fChase(0.f)
	, m_fJumpInitializeVelocity(0.f)
	, m_bIsJumping(false)
	, m_eState(STATE::STATE_END)
{
}

CMonsterAI::CMonsterAI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_fSightRange(0.f)
	, m_fAttackRange(0.f)
	, m_vRoamingCenter(_vec3(0.f, 0.f, 0.f))
	, m_fRoamingRadius(0.f)
	, m_fMaxTraceRadius(0.f)
	, m_pStateMachine(nullptr)
	, m_fDistance(0.f)
	, m_vDistance(_vec3(0.f, 0.f, 0.f))
	, m_fSpeed(0.f)
	, m_fChase(0.f)
	, m_fJumpInitializeVelocity(0.f)
	, m_bIsJumping(false)
	, m_eState(STATE::STATE_END)
{
}

CMonsterAI::CMonsterAI(const CMonsterAI& rhs)
	: CComponent(rhs)
	, m_fSightRange(rhs.m_fSightRange)
	, m_fAttackRange(rhs.m_fAttackRange)
	, m_vRoamingCenter(rhs.m_vRoamingCenter)
	, m_fRoamingRadius(rhs.m_fRoamingRadius)
	, m_fMaxTraceRadius(rhs.m_fMaxTraceRadius)
	, m_pStateMachine(rhs.m_pStateMachine)
	, m_fDistance(rhs.m_fDistance)
	, m_vDistance(rhs.m_vDistance)
	, m_fSpeed(rhs.m_fSpeed)
	, m_fChase(rhs.m_fChase)
	, m_fJumpInitializeVelocity(rhs.m_fChase)
	, m_bIsJumping(rhs.m_bIsJumping)
	, m_eState(rhs.m_eState)
{
}

CMonsterAI::~CMonsterAI()
{
}

void CMonsterAI::Init_Property(_float _fSight, _float _fAttack, _vec3 _vRoamingCen, _float _RoamingRad, _float _fMaxTrace)
{
	m_fSightRange = _fSight;
	m_fAttackRange = _fAttack;

	m_vRoamingCenter = _vRoamingCen;
	m_fRoamingRadius = _RoamingRad;

	m_fMaxTraceRadius = _fMaxTrace;
}

void CMonsterAI::Auto_Roaming()
{
}

void CMonsterAI::Auto_Attack()
{
}

void CMonsterAI::Back_Home()
{

}

_bool CMonsterAI::Out_Of_Town()
{
	if(m_fMaxTraceRadius < D3DXVec3Length(&(m_vRoamingCenter - m_pHost->m_pTransform->m_vInfo[INFO_POS])))
		return TRUE;
	return FALSE;
}

const _matrix* CMonsterAI::Compute_LookAtTarget(const _vec3* pTargetPos)
{
	///Look 벡터
	_vec3	vForward = m_pTransform->m_vInfo[INFO_LOOK];
	D3DXVec3Normalize(&vForward, &vForward);

	// Right 벡터
	_vec3	vRight = m_pTransform->m_vInfo[INFO_RIGHT];

	// Up벡터
	//_vec3 vDir = *pTargetPos - m_pTransform->m_vInfo[INFO_POS];

	_vec3	vUp = m_pTransform->m_vInfo[INFO_UP];
	D3DXVec3Normalize(&vUp, &vUp);

	_matrix matRot;
	D3DXMatrixIdentity(&matRot);
	// 항등행렬 초기화 

	matRot._11 = vRight.x;
	matRot._12 = vUp.x;
	matRot._13 = vForward.x;

	matRot._21 = vRight.y;
	matRot._22 = vUp.y;
	matRot._23 = vForward.y;

	matRot._31 = vRight.z;
	matRot._32 = vUp.z;
	matRot._33 = vForward.z;

	 //더 깔끔하게 할 수 있으면 바꿔주셔도 됩니다.


	return &matRot;

}

void CMonsterAI::Chase_Target(const _vec3* pTargetPos, const _float& fTimeDelta, const _float& fSpeed)
{
	_vec3 vDir = *pTargetPos - m_pTransform->m_vInfo[INFO_POS];

	_float fDistance = D3DXVec3Length(&vDir);

	if(fDistance > 3.f)
	m_pTransform->m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * fSpeed;

	_matrix matRot = *Compute_LookAtTarget(pTargetPos);
	_matrix matScale, matTrans;

	D3DXMatrixScaling(&matScale, 
		m_pTransform->m_vLocalScale.x, 
		m_pTransform->m_vLocalScale.y,
		m_pTransform->m_vLocalScale.z);

	D3DXMatrixTranslation(&matTrans,
		m_pTransform->m_vInfo[INFO_POS].x,
		m_pTransform->m_vInfo[INFO_POS].y,
		m_pTransform->m_vInfo[INFO_POS].z);

	_matrix matWorld = m_pTransform->WorldMatrix();

	matWorld = matScale * matRot * matTrans;

	m_pTransform->Set_WorldMatrix(matWorld);
}

HRESULT CMonsterAI::Ready_MonsterAI()
{	
	// Transform 초기화 이후 호출
	m_fSightRange = 3.f;
	m_fAttackRange = 2.f;
	
	//m_vRoamingCenter = static_cast<CTransform*>(m_pHost->m_pTransform)->m_vInfo[INFO_POS];
	m_fRoamingRadius = 5.f;

	return S_OK;
}

_int CMonsterAI::Update_Component(const _float& fTimeDelta)
{
	return _int();
}

CMonsterAI* CMonsterAI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonsterAI* pInstance = new CMonsterAI(pGraphicDev);

	if (FAILED(pInstance->Ready_MonsterAI()))
	{
		Safe_Release(pInstance);
		MSG_BOX("MonsterAI Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CMonsterAI::Clone(void)
{
	return new CMonsterAI(*this);
}

void CMonsterAI::Free()
{
	CComponent::Free();
}
