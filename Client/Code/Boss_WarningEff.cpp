#include "Boss_WarningEff.h"
#include "Export_Function.h"

CBoss_WarningEff::CBoss_WarningEff(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBoss_WarningEff::CBoss_WarningEff(const CBoss_WarningEff& rhs)
	: Engine::CGameObject(rhs)
{
}

CBoss_WarningEff::~CBoss_WarningEff()
{
	Free();
}

HRESULT CBoss_WarningEff::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fTime = 0.f;
	m_fScale = 2.5;
	m_pTransform->Scale(_vec3(m_fScale, 2.f, m_fScale));
	return S_OK;
}

_int CBoss_WarningEff::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);
	m_fScale -= fTimeDelta*2;
	m_fTime += fTimeDelta;
	if ((1.f < m_fTime))
	{
		m_fTime = 0.f;
		m_IsDead = true;
	}
	return iExit;
}

void CBoss_WarningEff::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(m_fScale, 2.f, m_fScale));
}

void CBoss_WarningEff::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

HRESULT CBoss_WarningEff::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossWarning"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBoss_WarningEff* CBoss_WarningEff::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_WarningEff* pInstance = new CBoss_WarningEff(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BossLightning Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBoss_WarningEff::Free()
{
	__super::Free();
}
