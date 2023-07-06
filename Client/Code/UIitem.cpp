#include "stdafx.h"
#include "..\Header\UIitem.h"

CUIitem::CUIitem(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIitem::~CUIitem()
{

}

HRESULT CUIitem::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // ÃÊ±âÈ­

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = 130.f;
	m_pTransform->m_vInfo[INFO_POS].y = 35.f;
	m_pTransform->m_vLocalScale.x = 100.f;
	m_pTransform->m_vLocalScale.y = 20.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	return S_OK;
}

_int CUIitem::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIitem::LateUpdate_Object(void)
{
	CTempUI::LateUpdate_Object();
}

void CUIitem::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIitem::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTexBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_HpBar"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIitem::Key_Input(void)
{
}

CUIitem* CUIitem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIitem*	pInstance = new CUIitem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUIitem::Free()
{
	CTempUI::Free();
}
