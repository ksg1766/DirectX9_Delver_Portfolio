#include "stdafx.h"
#include "Lava.h"

#include "Export_Function.h"

CLava::CLava(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCubeBlock(pGraphicDev)
{
}

CLava::CLava(const CLava& rhs)
	: CCubeBlock(rhs)
{
}

CLava::~CLava()
{
}

HRESULT CLava::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_byTextureNumber = 0;
	m_fDelay = 0.f;
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, -1.f, 0.f), &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	return S_OK;
}

_int CLava::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	m_fDelay += fTimeDelta;
	if (0.1f < m_fDelay)
	{
		(m_byTextureNumber < 31) ? (++m_byTextureNumber) : (m_byTextureNumber = 0);
		m_fDelay = 0.f;
	}
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	return iExit;
}

void CLava::LateUpdate_Object(void)
{
	//__super::LateUpdate_Object();
	m_pCollider->SetCenterPos(m_pTransform->m_vInfo[INFO_POS] - _vec3(0.f, 0.3f, 0.f));
}

void CLava::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pTexture->Render_Texture(m_byTextureNumber);
	m_pBuffer->Render_Buffer();
}

void CLava::OnCollisionEnter(CCollider* _pOther)
{
}

void CLava::OnCollisionStay(CCollider* _pOther)
{
}

void CLava::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CLava::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = static_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Lava"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CLava* CLava::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLava* pInstance = new CLava(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Lava Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLava::Free()
{
	__super::Free();
}
