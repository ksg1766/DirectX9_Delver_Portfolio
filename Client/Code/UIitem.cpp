#include "stdafx.h"
#include "..\Header\UIitem.h"
#include "UIequipmentslot.h"
#include "Player.h"
#include <UIbasicslot.h>
#include <UIemptyslot.h>

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
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vLocalScale.x = 23.f;
	m_pTransform->m_vLocalScale.y = 23.f;

	return S_OK;
}

_int CUIitem::Update_Object(const _float & fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIitem::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();

	Update_NumverUI();
}

void CUIitem::Render_Object()
{
	if(m_bUIDelete)
		Engine::UIManager()->Delete_FindItemUI(m_ItemID);

	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();

	// 숫자 개수 출력
	if (m_ItemID.iCount > 1 && !m_bMove)
	{
		_matrix      matWorld;

		if (m_iCurrentOneNum != 0)
		{
			D3DXMatrixIdentity(&matWorld);
			matWorld._11 = 4.2f;
			matWorld._22 = 5.1f;
			matWorld._41 = m_matWorld._41 + 8.f;
			matWorld._42 = m_matWorld._42 + 15.f;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNumberTextureCom->Render_Texture(m_iCurrentOneNum);
			m_pBufferCom->Render_Buffer();
		}

		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 4.2f;
		matWorld._22 = 5.1f;
		matWorld._41 = m_matWorld._41 + 17.f;
		matWorld._42 = m_matWorld._42 + 15.f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pNumberTextureCom->Render_Texture(m_iCurrentTwoNum);
		m_pBufferCom->Render_Buffer();
	}

	if (m_bTooltipRender)
	{
		_matrix      matWorld;

	
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = m_fTooltipPosX + 41.f;
		matWorld._42 = m_fTooltipPosY + 21.f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pTooltipCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CUIitem::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_itemUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pNumberTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_NumberUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE1, pComponent);

	pComponent = m_pTooltipCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_tooltipUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE1, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIitem::Key_Input(void)
{
	// 마우스가 UI를 움직이는 중이며 움직이는 UI가 아닐 시 리턴
	if (Engine::UIManager()->m_bMouse && !m_bOnlyMove)
		return;

	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	// 마우스가 해당 UI를 움직이고 있다면 마우스 좌표 값으로 대입
	if (m_bOnlyMove) 
	{
		m_pTransform->m_vInfo[INFO_POS].x = pt.x;
		m_pTransform->m_vInfo[INFO_POS].y = WINCY - pt.y;
		WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	}

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y)) {
		if (Engine::InputDev()->Mouse_Pressing(DIM_LB)) {
			// 마우스가 UI를 움직이는 상태가 아닐 시 
			if (!Engine::UIManager()->m_bMouse) {
				m_bMove = true;
				m_bOnlyMove = true;
				m_bTooltipRender = false;
				Engine::UIManager()->m_bMouse = true;
				Engine::UIManager()->Set_PickingItemUI(this);

				// 아이템 장착 아이템 타입인 경우 해당 UINumber 슬롯을 찾고 해당 슬롯 이미지 변경
				if (m_ItemID.eItemType == ITEMTYPE_GENERALITEM || m_ItemID.eItemType == ITEMTYPE_EQUIPITEM) {
					CGameObject* SlotObj = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIID_SLOTEQUIPMENT, m_UINumber);
					dynamic_cast<CUIequipmentslot*>(dynamic_cast<CTempUI*>(SlotObj))->Set_FindSlot(true);
				}
			}
		}
		else {
			if (m_bMove) {
				m_bMove = false;

				// 이동할 슬롯 주소를 가져와 할당
				CGameObject* ColliderSlotObj = Engine::UIManager()->Find_ColliderSlot();
				UIOBJECTTTAG UIColliderSlotObjID;
				_uint        UIColliderSlotNumber;

				CPlayer*    pPlayer    = dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
				CInventory* pInventory = dynamic_cast<CInventory*>(pPlayer->Get_Component(COMPONENTTAG::INVENTORY, ID_DYNAMIC));

				if (pPlayer == nullptr || pInventory == nullptr)
					return;

#pragma region 아이템 버리기
				if (ColliderSlotObj == nullptr) { // 이동할 슬롯을 찾지 못해 해당 아이템 버림 
					m_bOnlyMove = false;
					m_bTooltipRender = false;
					Engine::UIManager()->m_bMouse = false;
					Engine::UIManager()->Set_PickingItemUI(nullptr);

					if(m_ItemID.iCount == 1) { // 버리려는 아이템이 1개일 시 버리고 장착하고 있던 아이템일 시 장착 해제
						if (m_ItemID.eItemType == ITEMTYPE_WEAPONITEM && pPlayer->Get_CurrentEquipRight() != nullptr && m_ItemID.eItemID == dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight())->Get_ItemTag().eItemID)
						{ // 오른 손 무기 버렸을 시 
							pPlayer->Set_PrevEquipRight(nullptr);
							pPlayer->Set_CurrentEquipRight(nullptr);
							pPlayer->Set_ItemEquipRight(false);
						}
						else if (m_ItemID.eItemType == ITEMTYPE_GENERALITEM && pPlayer->Get_CurrentEquipLeft() != nullptr && m_ItemID.eItemID == dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipLeft())->Get_ItemTag().eItemID)
						{ // 왼 손 무기 버렸을 시 
							pPlayer->Set_PrevEquipLeft(nullptr);
							pPlayer->Set_CurrentEquipLeft(nullptr);
							pPlayer->Set_ItemEquipLeft(false);
						}

						UIOBJECTTTAG UIParentSlotObjID;
						_uint        UIParentSlotNumber;
						CGameObject* pSlotObj = Get_Parent();

						dynamic_cast<CTempUI*>(pSlotObj)->Get_UIObjID(UIParentSlotObjID, UIParentSlotNumber);
						dynamic_cast<CTempUI*>(pSlotObj)->Set_BeforeChild(dynamic_cast<CTempUI*>(Get_Parent())->Get_Child());
						dynamic_cast<CTempUI*>(pSlotObj)->Set_Child(nullptr);
						dynamic_cast<CTempUI*>(pSlotObj)->Set_EmptyBool(true);

						if (m_ItemID.eItemType != ITEMTYPE_GENERALITEM && m_ItemID.eItemType != ITEMTYPE_EQUIPITEM || UIParentSlotObjID != UIID_SLOTEQUIPMENT) {
							pInventory->delete_FindItem(m_ItemID);
						}
						else {
							dynamic_cast<CUIequipmentslot*>(pSlotObj)->Set_ThrowItem(m_ItemID.eItemID);
						}

						m_bUIDelete = true;
					}
				    else // 여러개일 시 한개씩 버림
				    {
						m_ItemID.iCount -= 1;
					   	pInventory->delete_FindItem(m_ItemID);
						m_pTransform->m_vInfo[INFO_POS].x = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].x;
						m_pTransform->m_vInfo[INFO_POS].y = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].y;
						WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
					}
				}
#pragma endregion 아이템 버리기
#pragma region 아이템 이동 및 스왑
				else if (ColliderSlotObj != nullptr) // 이동할 슬롯을 찾았을 시
				{
					_bool bSucess = false;
					dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_UIObjID(UIColliderSlotObjID, UIColliderSlotNumber);

					// 이동할 수 있는 슬롯인지 조건 검사
					switch (UIColliderSlotObjID)
					{
					case Engine::UIID_SLOTBASIC:
						bSucess = true;
						break;
					case Engine::UIID_SLOTEMPTY:
						bSucess = true;
						break;
					case Engine::UIID_SLOTEQUIPMENT:
						if (m_UINumber == UIColliderSlotNumber) { bSucess = true; }
						else                                    { bSucess = false; }
						break;
					}
					
					if (dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_EmptyBool() && bSucess) // 이동할 슬롯이 비어있고 장착해도 될 슬롯일 경우에 이동
					{
						UIOBJECTTTAG UIStartObjID;
						_uint        UIStartNumber;
						dynamic_cast<CTempUI*>(Get_Parent())->Get_UIObjID(UIStartObjID, UIStartNumber);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_BeforeChild(dynamic_cast<CTempUI*>(Get_Parent())->Get_Child());
						dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(nullptr);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);

						// 장착 성공 시 해당 정보 인벤토리에 업데이트 // 위치 이동 및 이전 위치에는 정보 초기화 (해제 X)
						pInventory->GoSwitch_InvenItem(m_ItemID, UIStartObjID, UIStartNumber, UIColliderSlotObjID, UIColliderSlotNumber);

						m_pTransform->m_vInfo[INFO_POS].x = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].x;
						m_pTransform->m_vInfo[INFO_POS].y = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].y;
						WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

						Set_Parent(ColliderSlotObj);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(this);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(false);

						// 장착하고 있는 아이템을 이동한 경우에 사용하고 있다는 슬롯 표시 이동
						if (m_ItemID.eItemType == ITEMTYPE_WEAPONITEM) {
							CGameObject* pRightItem = pPlayer->Get_CurrentEquipRight();
							if (pRightItem != nullptr) {

								ITEMTYPEID ItemId = dynamic_cast<CItem*>(pRightItem)->Get_ItemTag();
								if (ItemId.eItemID == m_ItemID.eItemID) {
									UIOBJECTTTAG UIFindObjID{};
									_uint        UIFindNumber;
									dynamic_cast<CTempUI*>(Get_Parent())->Get_UIObjID(UIFindObjID, UIFindNumber);

									if (UIFindObjID == UIID_SLOTBASIC)
										dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(true);
									else if (UIFindObjID == UIID_SLOTEMPTY)
										dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(true);
								}
							}
						}
					}
					else if (!dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_EmptyBool() && bSucess) // 이동할 슬롯이 비어있지 않고 장착해도 될 슬롯일 경우에 서로 스위칭
					{
						// 이동할 아이템 UI 및 부모 슬롯 주소
						ITEMTYPEID   UIStartItemID = m_ItemID;
						CGameObject* UIStartParentSlot = Get_Parent();
						UIOBJECTTTAG UIStartObjID;
						_uint        UIStartNumber;
						dynamic_cast<CTempUI*>(UIStartParentSlot)->Get_UIObjID(UIStartObjID, UIStartNumber);

						// 이동할 공간에 있는 아이템 UI를 가져오고 이전 자식으로 등록
						CGameObject* pGoItemUI = dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_Child();
						ITEMTYPEID   UIGoItemID = dynamic_cast<CUIitem*>(pGoItemUI)->Get_ItemTag();

						// 이동당하는 아이템이 해당 슬롯에 들어가도 되는 지 검사
						_bool bChangeSucess = false;
						switch (UIStartObjID)
						{
						case Engine::UIID_SLOTBASIC:
							bChangeSucess = true;
							break;
						case Engine::UIID_SLOTEMPTY:
							bChangeSucess = true;
							break;
						case Engine::UIID_SLOTEQUIPMENT:
							UIOBJECTTTAG UIEndObjID;
							_uint        UIEndNumber;
							dynamic_cast<CTempUI*>(pGoItemUI)->Get_UIObjID(UIEndObjID, UIEndNumber);
							if (UIEndNumber == UIStartNumber) { bChangeSucess = true; }
							else { bChangeSucess = false; }
							break;
						}

						if (bChangeSucess)
						{
							dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_BeforeChild(dynamic_cast<CTempUI*>(UIStartParentSlot)->Get_Child());
							dynamic_cast<CTempUI*>(ColliderSlotObj)->Set_BeforeChild(dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_Child());

							// 위치 이동
							m_pTransform->m_vInfo[INFO_POS].x = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].x;
							m_pTransform->m_vInfo[INFO_POS].y = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].y;
							WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

							pGoItemUI->m_pTransform->m_vInfo[INFO_POS].x = UIStartParentSlot->m_pTransform->m_vInfo[INFO_POS].x;
							pGoItemUI->m_pTransform->m_vInfo[INFO_POS].y = UIStartParentSlot->m_pTransform->m_vInfo[INFO_POS].y;
							dynamic_cast<CUIitem*>(pGoItemUI)->WorldMatrix(pGoItemUI->m_pTransform->m_vInfo[INFO_POS].x, pGoItemUI->m_pTransform->m_vInfo[INFO_POS].y, pGoItemUI->m_pTransform->m_vLocalScale.x, pGoItemUI->m_pTransform->m_vLocalScale.y);

							// 서로 부모 자식 설정
							if (UIColliderSlotObjID == UIID_SLOTEQUIPMENT) // 이동하는 곳이 장착 슬롯일 시 이전 상태 해제 후 해당 정보 업데이트 필요
							{
								// 2곳다 기존 장착 상태 해제
								// 재 장착 상태

								Set_Parent(ColliderSlotObj);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_NextChild(this);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(false);

								dynamic_cast<CUIitem*>(pGoItemUI)->Set_Parent(UIStartParentSlot);

								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_Child(pGoItemUI);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(false);

							}
							else if (UIStartObjID == UIID_SLOTEQUIPMENT)
							{
								Set_Parent(ColliderSlotObj);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(this);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(false);

								dynamic_cast<CUIitem*>(pGoItemUI)->Set_Parent(UIStartParentSlot);

								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_NextChild(pGoItemUI);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(false);
							}
							else
							{
								Set_Parent(ColliderSlotObj);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(this);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(false);

								dynamic_cast<CUIitem*>(pGoItemUI)->Set_Parent(UIStartParentSlot);

								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_Child(pGoItemUI);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(false);
							}

							// 장착 성공 시 해당 정보 인벤토리에 업데이트
							pInventory->ExSwitch_InvenItem(UIStartItemID, UIStartObjID, UIStartNumber, UIGoItemID, UIColliderSlotObjID, UIColliderSlotNumber);

							// 장착하고 있는 아이템을 이동한 경우 또는 이동 당한 경우에 불빛 조정
							CGameObject* pRightItem = pPlayer->Get_CurrentEquipRight();
							if (pRightItem != nullptr) {
								ITEMTYPEID ItemId = dynamic_cast<CItem*>(pRightItem)->Get_ItemTag();

								// 장착 아이템을 이동한 경우 이전 슬롯 불을 끄고 이동한 슬롯 불을 킨다.
								if (m_ItemID.eItemType == ITEMTYPE_WEAPONITEM && ItemId.eItemID == m_ItemID.eItemID)
								{
									// 이전 슬롯을 찾아 불을 끈다.
									if (UIStartObjID == UIID_SLOTBASIC) {
										dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(UIStartParentSlot))->Set_FindSlot(false);
									}
									else if (UIStartObjID == UIID_SLOTEMPTY) {
										dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(UIStartParentSlot))->Set_FindSlot(false);
									}

									// 이동한 슬롯을 찾으 불을 킨다.
									if (UIColliderSlotObjID == UIID_SLOTBASIC) {
										dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(true);
									}
									else if (UIColliderSlotObjID == UIID_SLOTEMPTY) {
										dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(true);
									}
								}
								// 장착 아이템이 이동 당한 경우 기존 이전 슬롯은 불을 끄고 이동당한 슬롯은 불을 킨다.
								else if (UIGoItemID.eItemType == ITEMTYPE_WEAPONITEM && ItemId.eItemID == UIGoItemID.eItemID)
								{
									// 이전 슬롯을 찾아 불을 끈다.
									if (UIColliderSlotObjID == UIID_SLOTBASIC) {
										dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(false);
									}
									else if (UIColliderSlotObjID == UIID_SLOTEMPTY) {
										dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(false);
									}

									// 이동한 슬롯을 찾으 불을 킨다.
									if (UIStartObjID == UIID_SLOTBASIC) {
										dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(UIStartParentSlot))->Set_FindSlot(true);
									}
									else if (UIStartObjID == UIID_SLOTEMPTY) {
										dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(UIStartParentSlot))->Set_FindSlot(true);
									}
								}
							}
						}
						else // 제자리로 이동
						{
							m_pTransform->m_vInfo[INFO_POS].x = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].x;
							m_pTransform->m_vInfo[INFO_POS].y = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].y;
							WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
						}
						
					}
					else // 이동 실패일 시 제자지로 이동
					{
						m_pTransform->m_vInfo[INFO_POS].x = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].x;
						m_pTransform->m_vInfo[INFO_POS].y = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].y;
						WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
					}

					m_bOnlyMove = false;
					Engine::UIManager()->m_bMouse = false;
					Engine::UIManager()->Set_PickingItemUI(nullptr);
				}

				// 초록불 들어온 슬롯 초기화
				if (m_ItemID.eItemType == ITEMTYPE_GENERALITEM || m_ItemID.eItemType == ITEMTYPE_EQUIPITEM) {
					CGameObject* SlotObj = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIID_SLOTEQUIPMENT, m_UINumber);
					dynamic_cast<CUIequipmentslot*>(dynamic_cast<CTempUI*>(SlotObj))->Set_FindSlot(false);
				}
			}
		}
#pragma endregion 아이템 이동 및 스왑

		// 움직이지 않고 해당 UI랑 충돌 중일 시 튤팁 출력
		if(!m_bMove) {
			m_fTooltipPosX = pt.x;
			m_fTooltipPosY = pt.y;
			m_bTooltipRender = true;
			Engine::UIManager()->Set_PickingItemUI(this);
		}
	}
	else { // 해당 UI랑 충돌 상태가 아닐 시 튤팁 등 해제
		m_bTooltipRender = false;
		if (Engine::UIManager()->Get_PickingItemUI() == this) {
			Engine::UIManager()->Set_PickingItemUI(nullptr);
		}
	}
}

void CUIitem::Update_NumverUI()
{
	_int iCurrentOneNum = m_ItemID.iCount / 10;
	_int iCurrentTwoNum = m_ItemID.iCount % 10;

	switch (iCurrentOneNum)
	{
	case 0:
		m_iCurrentOneNum = 0;
		break;
	case 1:
		m_iCurrentOneNum = 1;
		break;
	case 2:
		m_iCurrentOneNum = 2;
		break;
	case 3:
		m_iCurrentOneNum = 3;
		break;
	case 4:
		m_iCurrentOneNum = 4;
		break;
	case 5:
		m_iCurrentOneNum = 5;
		break;
	case 6:
		m_iCurrentOneNum = 6;
		break;
	case 7:
		m_iCurrentOneNum = 7;
		break;
	case 8:
		m_iCurrentOneNum = 8;
		break;
	case 9:
		m_iCurrentOneNum = 9;
		break;
	}

	switch (iCurrentTwoNum)
	{
	case 0:
		m_iCurrentTwoNum = 0;
		break;
	case 1:
		m_iCurrentTwoNum = 1;
		break;
	case 2:
		m_iCurrentTwoNum = 2;
		break;
	case 3:
		m_iCurrentTwoNum = 3;
		break;
	case 4:
		m_iCurrentTwoNum = 4;
		break;
	case 5:
		m_iCurrentTwoNum = 5;
		break;
	case 6:
		m_iCurrentTwoNum = 6;
		break;
	case 7:
		m_iCurrentTwoNum = 7;
		break;
	case 8:
		m_iCurrentTwoNum = 8;
		break;
	case 9:
		m_iCurrentTwoNum = 9;
		break;
	}
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
