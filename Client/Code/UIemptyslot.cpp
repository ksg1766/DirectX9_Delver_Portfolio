#include "stdafx.h"
#include "..\Header\UIemptyslot.h"

CUIemptyslot::CUIemptyslot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIemptyslot::~CUIemptyslot()
{

}

HRESULT CUIemptyslot::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // ÃÊ±âÈ­

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransform->m_vInfo[INFO_POS].x = 130.f;
	//m_pTransform->m_vInfo[INFO_POS].y = WINCY - 50.f;
	m_pTransform->m_vLocalScale.x = 30.f;
	m_pTransform->m_vLocalScale.y = 30.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_bEmpty = true;
	return S_OK;
}

_int CUIemptyslot::Update_Object(const _float & fTimeDelta)
{
	if (m_IsDead)
		return 0;

	if (m_bSetup) {
		m_bSetup = false;
		m_fCurrentImage = 0;
	}

	if (m_bEmpty)
		m_bEquip = false;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIemptyslot::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUIemptyslot::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIemptyslot::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EmptyslotUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIemptyslot::Key_Input(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y))
	{
		if (!m_bEquip)
		     m_fCurrentImage = 1;
		Engine::UIManager()->Set_ColliderSlot(m_UIObjID, m_UINumber, true);
	}
	else
	{
		if (!m_bEquip)
		     m_fCurrentImage = 0;
		Engine::UIManager()->Set_ColliderSlot(m_UIObjID, m_UINumber, false);
	}

}

CUIemptyslot* CUIemptyslot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIemptyslot*	pInstance = new CUIemptyslot(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUIemptyslot::Free()
{
	CTempUI::Free();
}
