#include "stdafx.h"
#include "..\Header\UIbasicslot.h"

CUIbasicslot::CUIbasicslot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIbasicslot::~CUIbasicslot()
{

}

HRESULT CUIbasicslot::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransform->m_vInfo[INFO_POS].x = 130.f;
	//m_pTransform->m_vInfo[INFO_POS].y = WINCY - 50.f;
	m_pTransform->m_vLocalScale.x = 30.f;
	m_pTransform->m_vLocalScale.y = 30.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_bEmpty = true;
	return S_OK;
}

_int CUIbasicslot::Update_Object(const _float & fTimeDelta)
{
	if (m_IsDead)
		return 0;

	if (m_bSetup) {
		m_bSetup = false;
		m_fCurrentImage = 0;

		switch (m_UINumber)
		{
		case 0:
			m_fCurrentNumber = 11;
			break;
		case 1:
			m_fCurrentNumber = 12;
			break;
		case 2:
			m_fCurrentNumber = 13;
			break;
		case 3:
			m_fCurrentNumber = 14;
			break;
		case 4:
			m_fCurrentNumber = 15;
			break;
		}
	}

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIbasicslot::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUIbasicslot::Render_Object()
{
	if (m_IsDead)
		return;
	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, 30.f, 30.f);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();

	// 일의 자리
	//m_pTransform->m_vInfo[INFO_POS].x = m_pTransform->m_vInfo[INFO_POS].x;
	//m_pTransform->m_vInfo[INFO_POS].y = m_pTransform->m_vInfo[INFO_POS].y;
	//m_pTransform->m_vLocalScale.x = ;
	//m_pTransform->m_vLocalScale.y = ;

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x - 14.f, m_pTransform->m_vInfo[INFO_POS].y - 10.f, 6.f, 10.f);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pNumberTextureCom->Render_Texture(m_fCurrentNumber);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIbasicslot::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTexBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EmptyslotUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pNumberTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_NumberUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE1, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIbasicslot::Key_Input(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y))
	{
		m_fCurrentImage = 1;
	}
	else
	{
		m_fCurrentImage = 0;
	}
}

CUIbasicslot* CUIbasicslot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIbasicslot*	pInstance = new CUIbasicslot(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUIbasicslot::Free()
{
	CTempUI::Free();
}
