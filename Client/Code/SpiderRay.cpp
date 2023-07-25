#include "..\Header\SpiderRay.h"
#include "Export_Function.h"
#include "DungeonSpider.h"
#include "Scene.h"

CSpiderRay::CSpiderRay(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSpiderRay::CSpiderRay(const CSpiderRay& rhs)
	: CGameObject(rhs)
{
}

CSpiderRay::~CSpiderRay()
{
	Free();
}

HRESULT CSpiderRay::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::RAY;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(0.00001f, 0.31f, 0.00001f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	return S_OK;
}

_int CSpiderRay::Update_Object(const _float& fTimeDelta)
{
	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	m_pTransform->m_vInfo[INFO_POS] = m_pHost->m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, -0.65f, 0.f);

	return iExit;
}

void CSpiderRay::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();

	if (!m_pColTarget)
		if (!static_cast<CDungeonSpider*>(m_pHost)->IsJump())
		{
			static_cast<CDungeonSpider*>(m_pHost)->Set_Jump(true);
			static_cast<CDungeonSpider*>(m_pHost)->Get_RigidBody()->UseGravity(true);
		}
}

void CSpiderRay::Render_Object(void)
{
#if _DEBUG
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pCollider->Render_Collider();
#endif // _DEBUG

}

void CSpiderRay::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOtherObj = _pOther->GetHost();
	
	if (OBJECTTAG::BLOCK == pOtherObj->Get_ObjectTag())
	{
		m_pColTarget = pOtherObj;
	}
}

void CSpiderRay::OnCollisionStay(CCollider* _pOther)
{
	CGameObject* pOtherObj = _pOther->GetHost();
	if (OBJECTTAG::BLOCK == pOtherObj->Get_ObjectTag())
	{
		static_cast<CDungeonSpider*>(m_pHost)->Set_Jump(false);
		static_cast<CDungeonSpider*>(m_pHost)->Get_RigidBody()->UseGravity(false);
	}
}

void CSpiderRay::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOtherObj = _pOther->GetHost();
	if (OBJECTTAG::BLOCK == pOtherObj->Get_ObjectTag())
	{
		m_pColTarget = nullptr;
		static_cast<CDungeonSpider*>(m_pHost)->Set_Jump(true);
		static_cast<CDungeonSpider*>(m_pHost)->Get_RigidBody()->UseGravity(true);
	}
}

HRESULT CSpiderRay::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CSpiderRay* CSpiderRay::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpiderRay* pInstance = new CSpiderRay(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CSpiderRay*>(pInstance);

		MSG_BOX("SpiderRay Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSpiderRay::Free()
{
	__super::Free();
}
