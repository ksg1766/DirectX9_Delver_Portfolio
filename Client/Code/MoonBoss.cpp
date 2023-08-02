#include "stdafx.h"
#include "..\Header\MoonBoss.h"

#include "Export_Function.h"

CMoonBoss::CMoonBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMoonBoss::CMoonBoss(const CMoonBoss& rhs)
	: CGameObject(rhs)
{
}

CMoonBoss::~CMoonBoss()
{
}

HRESULT CMoonBoss::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::IMMORTAL;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Translate(_vec3(-125.f, 75.f, 1.f));

	return S_OK;
}

_int CMoonBoss::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CMoonBoss::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(13.f, 13.f, 13.f));
}

void CMoonBoss::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);

	m_pTextureCom->Render_Texture(1);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
}

HRESULT CMoonBoss::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Moon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBillBoardCom = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CMoonBoss* CMoonBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMoonBoss* pInstance = new CMoonBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CMoonBoss Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMoonBoss::Free()
{
	__super::Free();
}
