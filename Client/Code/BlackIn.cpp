#include "stdafx.h"
#include "..\Header\BlackIn.h"

CBlackIn::CBlackIn(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CBlackIn::~CBlackIn()
{
	Free();
}

HRESULT CBlackIn::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // ÃÊ±âÈ­

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Engine::UIManager()->Hide_PopupUI(POPUP_MOUSE);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY / 2;
	m_pTransform->m_vLocalScale.x = 650;
	m_pTransform->m_vLocalScale.y = 370;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	
	return S_OK;
}

Engine::_int CBlackIn::Update_Object(const _float& fTimeDelta)
{
	//_int iExit = CTempUI::Update_Object(fTimeDelta);
	if (m_IsDead)
		return 0;

	m_fFrame -= 1.f * fTimeDelta * 10.f;

	if (0.f >= m_fFrame)
	{
		m_fFrame = 0.f;
		Engine::UIManager()->Show_PopupUI(POPUP_MOUSE);
		//Engine::UIManager()->Delete_BasicObject(Engine::UILAYER::UI_UP);
		m_IsDead = true;
	}

	return 0;
}

void CBlackIn::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CBlackIn::Render_Object(void)
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CBlackIn::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_FadeIn"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}


void CBlackIn::Key_Input(void)
{
}

void CBlackIn::Free()
{
	CTempUI::Free();
}

CBlackIn* CBlackIn::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBlackIn*	pInstance = new CBlackIn(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}

	return pInstance;
}
