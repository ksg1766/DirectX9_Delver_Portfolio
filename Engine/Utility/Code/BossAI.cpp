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

void CBossAI::Auto_Roaming()
{

}

void CBossAI::Auto_Attack()
{
}

void CBossAI::Back_Home()
{

}

void CBossAI::Chase_Player(const _float& fTimeDelta)
{
	_vec3  pPlayerPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
	_vec3	Dir = pPlayerPos - _vec3(-4.f, 0.f, -4.f);
	float Dot = D3DXVec3Dot(&pPlayerPos, &Dir);

	/*if (100 < Dot)
	{
		m_pTransform->m_vInfo[INFO_POS] = _vec3(-4.f, 0.f, -4.f);
	}
	else if (100 >= Dot)
	{
		m_pTransform->Translate(5.f * fTimeDelta * -m_pTransform->m_vInfo[INFO_LOOK]);
	}*/
}

_bool CBossAI::Out_Of_Town()
{
	if(m_fMaxTraceRadius < D3DXVec3Length(&(m_vRoamingCenter - m_pHost->m_pTransform->m_vInfo[INFO_POS])))
		return TRUE;
	return FALSE;
}

HRESULT CBossAI::Ready_MonsterAI()
{	// Transform 초기화 이후 호출
	m_fSightRange = 3.f;
	m_fAttackRange = 2.f;
	
	m_vRoamingCenter = dynamic_cast<CTransform*>(m_pHost->m_pTransform)->m_vInfo[INFO_POS];
	m_fRoamingRadius = 5.f;

	return S_OK;
}

_int CBossAI::Update_Component(const _float& fTimeDelta)
{
	return _int();
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
