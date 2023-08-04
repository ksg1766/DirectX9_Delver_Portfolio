#include "KingSpiderFog.h"
#include "Export_Function.h"

CKingSpiderFog::CKingSpiderFog(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CKingSpiderFog::CKingSpiderFog(const CKingSpiderFog& rhs)
	: CGameObject(rhs)
{
}

CKingSpiderFog::~CKingSpiderFog()
{
	Free();
}

HRESULT CKingSpiderFog::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::EFFECT;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fScale = 2.f;
	m_fDelay = 0.f;
	return S_OK;
}

_int CKingSpiderFog::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);
	m_fDelay += fTimeDelta;
	m_fScale -= fTimeDelta;
	if (2.f < m_fDelay)
	{
		m_fDelay = 0.f;
		m_IsDead = true;
	}

	return iExit;
}

void CKingSpiderFog::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(m_fScale, m_fScale, m_fScale));
}

void CKingSpiderFog::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();
}

HRESULT CKingSpiderFog::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pBuffer = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectSpiderFog"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);
	return S_OK;
}

CKingSpiderFog* CKingSpiderFog::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CKingSpiderFog* pGameObject = new CKingSpiderFog(pGraphicDev);
	if (FAILED(pGameObject->Ready_Object()))
	{
		Safe_Release(pGameObject);
		MSG_BOX("KingSpider Fog create Failed");
		return nullptr;
	}
	return pGameObject;
}

void CKingSpiderFog::Free()
{
	__super::Free();
}
