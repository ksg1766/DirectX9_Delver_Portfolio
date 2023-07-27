#include "Boss_CautionEff.h"
#include "Export_Function.h"

CBoss_CautionEff::CBoss_CautionEff(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBoss_CautionEff::CBoss_CautionEff(const CBoss_CautionEff& rhs)
	: Engine::CGameObject(rhs)
{
}

CBoss_CautionEff::~CBoss_CautionEff()
{
	Free();
}

HRESULT CBoss_CautionEff::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fTime = 0.f;
	m_fScale = 1.;
	return S_OK;
}

_int CBoss_CautionEff::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);
	m_fScale -= fTimeDelta;
	m_fTime += fTimeDelta;
	if ((1.f < m_fTime))
	{
		m_fTime = 0.f;
		m_IsDead = true;
	}
	m_pTransform->Scale(_vec3(m_fScale, 1.f, m_fScale));
	return iExit;
}

void CBoss_CautionEff::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CBoss_CautionEff::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();

	m_pTransform->Rotate(ROT_Y, D3DXToRadian(90.f));
	_matrix matWorld = m_pTransform->WorldMatrix();
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(-90.f));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBoss_CautionEff::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossCaution"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);
	
	return S_OK;
}

CBoss_CautionEff* CBoss_CautionEff::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_CautionEff* pInstance = new CBoss_CautionEff(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BossLightning Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBoss_CautionEff::Free()
{
	__super::Free();
}
