#include "Export_Utility.h"

#include "../../../Client/Header/UIitem.h"

IMPLEMENT_SINGLETON(CUIManager)

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
	Free();
}

void CUIManager::Show_PopupUI(UIPOPUPLAYER _PopupID)
{
	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto& iter : m_mapPpopupUI[_PopupID][i])
		{
			iter->Set_Dead(false);
		}
	}
}

void CUIManager::Hide_PopupUI(UIPOPUPLAYER _PopupID)
{
	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto& iter : m_mapPpopupUI[_PopupID][i])
		{
			iter->Set_Dead(true);
		}
	}
}

void CUIManager::Delete_BasicObject(UILAYER eType)
{
	for (auto& obj : m_vecUIbasic[eType]){
		m_vecDead.push_back(obj); 
	}
	m_vecUIbasic[eType].clear();
}

void CUIManager::Delete_FindItemUI(ITEMTYPEID _itemId)
{
	// 해당 아이디의 아이템을 찾아와서 감소 및 삭제
	// 기본 슬롯 5개 먼저 검사
	for (auto& iter : m_vecBasicItem) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CUIitem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemType == _itemId.eItemType)
			{
				// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
				if (SlotItemType.iCount > _itemId.iCount)
				{
					dynamic_cast<CUIitem*>(iter)->Remove_ItemCount(_itemId.iCount);
					return;
				}
				else
				{
					dynamic_cast<CTempUI*>(dynamic_cast<CUIitem*>(iter)->Get_Parent())->Set_EmptyBool(true);
					Safe_Release<CGameObject*>(iter);
					iter = nullptr;
					return;
				}
			}
		}
	}
	// 내부 슬롯 18개 검사
	for (auto& iter : m_mapPpopupUI[POPUP_INVEN][UI_MIDDLE]) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CUIitem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemType == _itemId.eItemType)
			{
				// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
				if (SlotItemType.iCount > _itemId.iCount)
				{
					dynamic_cast<CItem*>(iter)->Remove_ItemCount(_itemId.iCount);
					return;
				}
				else
				{
					dynamic_cast<CTempUI*>(dynamic_cast<CUIitem*>(iter)->Get_Parent())->Set_EmptyBool(true);
					Safe_Release<CGameObject*>(iter);
					iter = nullptr;
					return;
				}
			}
		}
	}
}

void CUIManager::AddBasicGameobject_UI(UILAYER eType, CGameObject* pGameObject)
{
	if (UILAYER::UI_END <= eType || nullptr == pGameObject)
		return;

	m_vecUIbasic[eType].push_back(pGameObject);
	//pGameObject->AddRef();
}

void CUIManager::AddBasicItemGameobject_UI(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return;

	m_vecBasicItem.push_back(pGameObject);
	//pGameObject->AddRef();
}

void CUIManager::AddPopupGameobject_UI(UIPOPUPLAYER ePopupLayer, UILAYER eType, CGameObject* pGameObject)
{
	if (UIPOPUPLAYER::POPUP_END <= ePopupLayer || UILAYER::UI_END <= eType || nullptr == pGameObject)
		return;

	m_mapPpopupUI[ePopupLayer][eType].push_back(pGameObject);
}

void CUIManager::AddItemGameobject_UI(CGameObject* pGameObject)
{
	// 들어온 아이템 오브젝트 아이템 타입 및 아이디, 개수를 가져옴.
	ITEMTYPEID ItemType = dynamic_cast<CUIitem*>(pGameObject)->Get_ItemTag();

	// 이미 보유하고 있는 아이템인지 검사한다.
	// 기본 슬롯 5개 먼저 검사
	for (auto iter : m_vecBasicItem) {
		if (iter != nullptr)
		{
			ITEMTYPEID SlotItemType = dynamic_cast<CUIitem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemType == ItemType.eItemType)
			{
				// 같은 아이템이 존재할 시 해당 개수만큼 카운트 증가 후 들어온 아이템 삭제
				dynamic_cast<CUIitem*>(iter)->Add_ItemCount(ItemType.iCount);
				Safe_Release<CGameObject*>(pGameObject);
				return;
			}
		}
	}
	// 내부 슬롯 18개 검사
	for (auto iter : m_mapPpopupUI[POPUP_INVEN][UI_MIDDLE]) {
		if (iter != nullptr)
		{
			ITEMTYPEID SlotItemType = dynamic_cast<CUIitem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemType == ItemType.eItemType)
			{
				// 같은 아이템이 존재할 시 해당 개수만큼 카운트 증가 후 들어온 아이템 삭제
				dynamic_cast<CUIitem*>(iter)->Add_ItemCount(ItemType.iCount);
				Safe_Release<CGameObject*>(pGameObject);
				return;
			}
		}
	}
	
	// 빈 공간이 있는지 검사후 할당 후 장착
	for (auto iter : m_vecUIbasic[UI_DOWN])
	{
		if (dynamic_cast<CTempUI*>(iter)->Get_EmptyBool())
		{
			pGameObject->m_pTransform->m_vInfo[INFO_POS].x = iter->m_pTransform->m_vInfo[INFO_POS].x;
			pGameObject->m_pTransform->m_vInfo[INFO_POS].y = iter->m_pTransform->m_vInfo[INFO_POS].y;
			dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);

			dynamic_cast<CTempUI*>(pGameObject)->Set_Parent(iter);
			dynamic_cast<CTempUI*>(iter)->Set_EmptyBool(false);

			Engine::UIManager()->AddBasicItemGameobject_UI(pGameObject);
			return;
		}
	}

	for (auto iter : m_mapPpopupUI[POPUP_INVEN][UI_DOWN])
	{
		if (dynamic_cast<CTempUI*>(iter)->Get_EmptyBool())
		{
			pGameObject->m_pTransform->m_vInfo[INFO_POS].x = iter->m_pTransform->m_vInfo[INFO_POS].x;
			pGameObject->m_pTransform->m_vInfo[INFO_POS].y = iter->m_pTransform->m_vInfo[INFO_POS].y;
			dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);

			dynamic_cast<CTempUI*>(pGameObject)->Set_Parent(iter);
			dynamic_cast<CTempUI*>(iter)->Set_EmptyBool(false);

			Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_INVEN, Engine::UILAYER::UI_MIDDLE, pGameObject);
			return;
		}
	}
}

_int CUIManager::Update_UI(const _float& fTimeDelta)
{
 	for_each(m_vecDead.begin(), m_vecDead.end(), CDeleteObj());
	m_vecDead.clear();

	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto iter : m_vecUIbasic[i])
		{
			if(iter != nullptr)
				iter->Update_Object(fTimeDelta);
		}
	}

	for (auto& Mapiter : m_mapPpopupUI)
	{
		for (size_t i = 0; i < UILAYER::UI_END; ++i)
		{
			for (auto iter : Mapiter.second[i])
				if (iter != nullptr)
					iter->Update_Object(fTimeDelta);
		}
	}

	for (auto iter : m_vecBasicItem)
	{
		if (iter != nullptr)
			iter->Update_Object(fTimeDelta);
	}

	return _int();
}

void CUIManager::LateUpdate_UI()
{
	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto iter : m_vecUIbasic[i])
			if (iter != nullptr)
				iter->LateUpdate_Object();
	}

	for (auto& Mapiter : m_mapPpopupUI)
	{
		for (size_t i = 0; i < UILAYER::UI_END; ++i)
		{
			for (auto iter : Mapiter.second[i])
				if (iter != nullptr)
					iter->LateUpdate_Object();
		}
	}

	for (auto iter : m_vecBasicItem)
	{
		if (iter != nullptr)
			iter->LateUpdate_Object();
	}
}

void CUIManager::Render_UI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	D3DVIEWPORT9 m_BackupViewPort;
	pGraphicDev->GetViewport(&m_BackupViewPort); // 현재 뷰 포트 백업

	// UI용 새로운 뷰 포트 생성 및 적용
	D3DVIEWPORT9 UiViewPort;
	UiViewPort.X = 0;
	UiViewPort.Y = 0;
	UiViewPort.Width = WINCX;
	UiViewPort.Height = WINCY;
	UiViewPort.MinZ = 0;
	UiViewPort.MaxZ = 0;
	pGraphicDev->SetViewport(&UiViewPort);

	_matrix matView;
	pGraphicDev->SetTransform(D3DTS_VIEW, D3DXMatrixIdentity(&matView)); // 항등행렬로 적용된 뷰 행렬 초기화.

	_matrix	m_matProj;
	D3DXMatrixOrthoOffCenterLH(&m_matProj, 0, WINCX, 0, WINCY, -1.f, 1.f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);   // 직교투영 행렬 적용.

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);    // Z버퍼 OFF

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파렌더링 ON
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto iter : m_vecUIbasic[i])
			if (iter != nullptr)
				iter->Render_Object();
	}

	for (auto& Mapiter : m_mapPpopupUI)
	{
		for (size_t i = 0; i < UILAYER::UI_END; ++i)
		{
			for (auto iter : Mapiter.second[i])
				if (iter != nullptr)
					iter->Render_Object();
		}
	}

	for (auto iter : m_vecBasicItem)
	{
		if (iter != nullptr)
			iter->Render_Object();
	}

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // 알파렌더링 OFF

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      // Z버퍼 ON


	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);    // UI 전체 출력 후 다시 원근투영 행렬 적용.


	pGraphicDev->SetViewport(&m_BackupViewPort);                // UI 전체 출력 후 백업해둔 이전 뷰포트로 되돌림.
}

void CUIManager::Free()
{
	for_each(m_vecDead.begin(), m_vecDead.end(), CDeleteObj());
	m_vecDead.clear();

	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto iter : m_vecUIbasic[i])
			if (iter != nullptr)
				Safe_Release(iter);
	}

	for (auto& Mapiter : m_mapPpopupUI) 
	{
		for (size_t i = 0; i < UILAYER::UI_END; ++i)
		{
			for_each(Mapiter.second[i].begin(), Mapiter.second[i].end(), CDeleteObj());
			Mapiter.second[i].clear();
		}
	}

	for (auto iter : m_vecBasicItem)
		if (iter != nullptr)
			Safe_Release(iter);

	m_vecBasicItem.clear();
}