#include "stdafx.h"
#include "..\Header\StartButton.h"

#include "StageLoading.h"

CStartButton::CStartButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CStartButton::~CStartButton()
{
}

HRESULT CStartButton::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // ÃÊ±âÈ­

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = 400.f;
	m_pTransform->m_vLocalScale.x = 200;
	m_pTransform->m_vLocalScale.y = 40;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	
	m_fCurrentImage = 4;

	return S_OK;
}

Engine::_int CStartButton::Update_Object(const _float& fTimeDelta)
{
	//_int iExit = CTempUI::Update_Object(fTimeDelta);

	return 0;
}

void CStartButton::LateUpdate_Object(void)
{
	Key_Input();

	CTempUI::LateUpdate_Object();
}

void CStartButton::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();
}

HRESULT CStartButton::Add_Component(void)
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


void CStartButton::Key_Input(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y))
	{
		m_fCurrentImage = 5;
		if (Engine::InputDev()->Mouse_Down(DIM_LB)) {

			Engine::UIManager()->Delete_BasicObject(Engine::UILAYER::UI_DOWN);
			Engine::UIManager()->Delete_BasicObject(Engine::UILAYER::UI_MIDDLE);
			Engine::UIManager()->Delete_BasicObject(Engine::UILAYER::UI_UP);

			CScene* pScene = CStageLoading::Create(m_pGraphicDev);
			Engine::SceneManager()->Set_Scene(pScene);
		}
	}
	else
	{
		m_fCurrentImage = 4;
	}
}

void CStartButton::Free()
{
	CTempUI::Free();
}

CStartButton* CStartButton::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStartButton*	pInstance = new CStartButton(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}

	return pInstance;
}
