#include "stdafx.h"
#include "..\Header\UIbutton.h"

CUIbutton::CUIbutton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIbutton::~CUIbutton()
{

}

HRESULT CUIbutton::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransform->m_vInfo[INFO_POS].x = 130.f;
	//m_pTransform->m_vInfo[INFO_POS].y = 35.f;
	m_pTransform->m_vLocalScale.x = 30.f;
	m_pTransform->m_vLocalScale.y = 20.f;

	//WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	return S_OK;
}

_int CUIbutton::Update_Object(const _float & fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIbutton::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUIbutton::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIbutton::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_InvenButtonUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIbutton::Key_Input(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	CGameObject* Obj = nullptr;

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y))
	{
		if (Engine::InputDev()->Mouse_Down(DIM_LB))
		{
			int a = 0;
			switch (m_UINumber)
			{
			case 0: // 인벤토리 버튼
				 // 눌린 상태 해제
				Obj = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 1);
				static_cast<CTempUI*>(Obj)->Set_UIImage(3);
				Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_STAT);
				Engine::UIManager()->m_bStat = false;
				// 눌린 상태
				m_fCurrentImage = 0;
				Engine::UIManager()->Show_PopupUI(Engine::UIPOPUPLAYER::POPUP_INVEN);
				Engine::UIManager()->Show_InvenItem(1);
				Engine::UIManager()->m_bInven = true;
				break;
			case 1: // 스탯창 버튼
				// 눌린 상태 해제
				Obj = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 0);
				static_cast<CTempUI*>(Obj)->Set_UIImage(1);
				Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_INVEN);
				Engine::UIManager()->Hide_InvenItem(1);
				Engine::UIManager()->m_bInven = false;
				// 눌린 상태
				m_fCurrentImage = 2;
				Engine::UIManager()->Show_PopupUI(Engine::UIPOPUPLAYER::POPUP_STAT);
				Engine::UIManager()->m_bStat = true;
				break;
			}
			//m_bCollider = true;
		}
	}
}

CUIbutton* CUIbutton::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIbutton*	pInstance = new CUIbutton(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUIbutton::Free()
{
	CTempUI::Free();
}
