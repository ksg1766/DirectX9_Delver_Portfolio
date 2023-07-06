#include "stdafx.h"
#include "..\Header\UItooltip.h"

CUItooltip::CUItooltip(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUItooltip::~CUItooltip()
{

}

HRESULT CUItooltip::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // ÃÊ±âÈ­

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = 400.f;
	m_pTransform->m_vInfo[INFO_POS].y = 300.f;
	m_pTransform->m_vLocalScale.x = 80.f;
	m_pTransform->m_vLocalScale.y = 40.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	return S_OK;
}

_int CUItooltip::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUItooltip::LateUpdate_Object(void)
{
	CTempUI::LateUpdate_Object();
}

void CUItooltip::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUItooltip::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTexBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_tooltipUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUItooltip::Key_Input(void)
{
}

CUItooltip* CUItooltip::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUItooltip*	pInstance = new CUItooltip(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUItooltip::Free()
{
	CTempUI::Free();
}
