#include "stdafx.h"
#include "..\Header\EditButton.h"

#include "EditLoading.h"

CEditButton::CEditButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CEditButton::~CEditButton()
{
}

HRESULT CEditButton::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BACKGROUND;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // ÃÊ±âÈ­

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = 300.f;
	m_pTransform->m_vLocalScale.x = 200;
	m_pTransform->m_vLocalScale.y = 40;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	
	m_fCurrentImage = 6;

	return S_OK;
}

Engine::_int CEditButton::Update_Object(const _float& fTimeDelta)
{
	//_int iExit = CTempUI::Update_Object(fTimeDelta);

	return 0;
}

void CEditButton::LateUpdate_Object(void)
{
	Key_Input();

	CTempUI::LateUpdate_Object();
}

void CEditButton::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEditButton::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}


void CEditButton::Key_Input(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y))
	{
		m_fCurrentImage = 7;
		if (Engine::InputDev()->Mouse_Down(DIM_LB)) {

			Engine::UIManager()->Delete_BasicObject(Engine::UILAYER::UI_DOWN);
			Engine::UIManager()->Delete_BasicObject(Engine::UILAYER::UI_MIDDLE);
			Engine::UIManager()->Delete_BasicObject(Engine::UILAYER::UI_UP);

			CScene* pScene = CEditLoading::Create(m_pGraphicDev);
			Engine::SceneManager()->Set_Scene(pScene);
		}
	}
	else
	{
		m_fCurrentImage = 6;
	}
}

void CEditButton::Free()
{
	CTempUI::Free();
}

CEditButton* CEditButton::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEditButton* pInstance = new CEditButton(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}

	return pInstance;
}
