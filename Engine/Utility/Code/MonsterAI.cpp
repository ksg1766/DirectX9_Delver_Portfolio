#include "..\..\Header\MonsterAI.h"
#include "Transform.h"
#include "GameObject.h"


CMonsterAI::CMonsterAI()
	:m_fSightRange(0.f)
	, m_fAttackRange(0.f)
	, m_vRoamingCenter(_vec3(0.f, 0.f, 0.f))
	, m_fRoamingRadius(0.f)
	, m_fMaxTraceRadius(0.f)
{
}

CMonsterAI::CMonsterAI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_fSightRange(0.f)
	, m_fAttackRange(0.f)
	, m_vRoamingCenter(_vec3(0.f, 0.f, 0.f))
	, m_fRoamingRadius(0.f)
	, m_fMaxTraceRadius(0.f)
{
}

CMonsterAI::CMonsterAI(const CMonsterAI& rhs)
	: CComponent(rhs)
	, m_fSightRange(rhs.m_fSightRange)
	, m_fAttackRange(rhs.m_fAttackRange)
	, m_vRoamingCenter(rhs.m_vRoamingCenter)
	, m_fRoamingRadius(rhs.m_fRoamingRadius)
	, m_fMaxTraceRadius(rhs.m_fMaxTraceRadius)
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

HRESULT CMonsterAI::Ready_MonsterAI()
{	// Transform 초기화 이후 호출
	m_fSightRange = 3.f;
	m_fAttackRange = 2.f;
	
	m_vRoamingCenter = dynamic_cast<CTransform*>(m_pHost->m_pTransform)->m_vInfo[INFO_POS];
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
