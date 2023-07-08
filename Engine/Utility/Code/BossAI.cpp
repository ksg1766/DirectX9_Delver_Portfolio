#include "..\..\Header\BossAI.h"
#include "Transform.h"
#include "GameObject.h"
#include "Export_Utility.h"

CBossAI::CBossAI()
	:m_fSightRange(0.f)
	, m_fAttackRange(0.f)
	, m_vRoamingCenter(_vec3(0.f, 0.f, 0.f))
	, m_fRoamingRadius(0.f)
	, m_fMaxTraceRadius(0.f)
{
}

CBossAI::CBossAI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_fSightRange(0.f)
	, m_fAttackRange(0.f)
	, m_vRoamingCenter(_vec3(0.f, 0.f, 0.f))
	, m_fRoamingRadius(0.f)
	, m_fMaxTraceRadius(0.f)
{
}

CBossAI::CBossAI(const CBossAI& rhs)
	: CComponent(rhs)
	, m_fSightRange(rhs.m_fSightRange)
	, m_fAttackRange(rhs.m_fAttackRange)
	, m_vRoamingCenter(rhs.m_vRoamingCenter)
	, m_fRoamingRadius(rhs.m_fRoamingRadius)
	, m_fMaxTraceRadius(rhs.m_fMaxTraceRadius)
{
}

CBossAI::~CBossAI()
{
}

void CBossAI::Init_Property(_float _fSight, _float _fAttack, _vec3 _vRoamingCen, _float _RoamingRad, _float _fMaxTrace)
{
	m_fSightRange = _fSight;
	m_fAttackRange = _fAttack;

	m_vRoamingCenter = _vRoamingCen;
	m_fRoamingRadius = _RoamingRad;

	m_fMaxTraceRadius = _fMaxTrace;
}


HRESULT CBossAI::Ready_MonsterAI()
{	// Transform 초기화 이후 호출
	m_fSightRange = 3.f;
	m_fAttackRange = 2.f;
	
	//m_vRoamingCenter = dynamic_cast<CTransform*>(m_pHost->m_pTransform)->m_vInfo[INFO_POS];
	m_fRoamingRadius = 5.f;

	return S_OK;
}

_int CBossAI::Update_Component(const _float& fTimeDelta)
{
	
	return _int();
}

void CBossAI::Teleport()
{
	srand(unsigned(time(NULL)));
	_float fDirX, fDirZ = 0.f;
	fDirX = rand()% 6;
	fDirZ = rand() % 6;
	int iA;
	switch(rand() % 2)
	{
	case 0:
		iA = -1;
		break;
	case 1:
		iA = 1;
		break;
	}
	m_pHost->m_pTransform->m_vInfo[INFO_POS].x += fDirX * iA;
	m_pHost->m_pTransform->m_vInfo[INFO_POS].z += fDirZ * iA;
}

void CBossAI::Chase_Player(const _float& fTimeDelta)
{
}

CBossAI* CBossAI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossAI* pInstance = new CBossAI(pGraphicDev);

	if (FAILED(pInstance->Ready_MonsterAI()))
	{
		Safe_Release(pInstance);
		MSG_BOX("MonsterAI Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CBossAI::Clone(void)
{
	return new CBossAI(*this);
}

void CBossAI::Free()
{
	CComponent::Free();
}
