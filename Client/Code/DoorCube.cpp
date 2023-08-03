#include "stdafx.h"
#include "SoundManager.h"
#include "Export_Function.h"
#include "Player.h"
#include "DoorCube.h"

CDoorCube::CDoorCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CDoorCube::CDoorCube(const CDoorCube& rhs)
	: Engine::CGameObject(rhs)
{
}

CDoorCube::~CDoorCube()
{
}

HRESULT CDoorCube::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::DOORBLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(1.f, 1.f, 1.f));
	//m_pTransform->Translate(_vec3(0.f, 4.f, -60.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale());

	return S_OK;
}

_int CDoorCube::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	_uint iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CDoorCube::LateUpdate_Object()
{
	__super::LateUpdate_Object();

}

void CDoorCube::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(39);
	m_pCubeBf->Render_Buffer();
}

void CDoorCube::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);
}

void CDoorCube::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);
}

void CDoorCube::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionExit(_pOther);
}

HRESULT CDoorCube::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Cube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);


	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CDoorCube* CDoorCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoorCube* pInstance = new CDoorCube(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CDoorCube*>(pInstance);

		MSG_BOX("Create DoorCube FAILED");
		return nullptr;
	}

	return pInstance;
}

void CDoorCube::Free()
{
	__super::Free();
}
