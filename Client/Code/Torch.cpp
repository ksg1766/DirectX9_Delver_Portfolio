#include "Torch.h"
#include "Export_Function.h"

CTorch::CTorch(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CTorch::CTorch(const CTorch& rhs)
	:CGameObject(rhs)
{
}

CTorch::~CTorch()
{
}

HRESULT CTorch::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::IMMORTAL;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fFrame = 0.f;
	return S_OK;
}

_int CTorch::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);
	m_fFrame += fTimeDelta * 16.f;
	if (8.f < m_fFrame)
		m_fFrame = 0.f;
	return iExit;
}

void CTorch::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
	m_pBillBoard->LateUpdate_Component();
}

void CTorch::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(_uint(m_fFrame));
	m_pBuffer->Render_Buffer();
}

HRESULT CTorch::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Torch"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBillBoard  = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);
	return S_OK;
}

CTorch* CTorch::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTorch* pGameObject = new CTorch(pGraphicDev);
	if (FAILED(pGameObject->Ready_Object()))
	{
		Safe_Release(pGameObject);
		MSG_BOX("Torch create Failed");
		return nullptr;
	}

	return pGameObject;
}

void CTorch::Free()
{
	__super::Free();
}
