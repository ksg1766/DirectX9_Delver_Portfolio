#include "stdafx.h"
#include "..\Header\UIitem.h"
#include "UIequipmentslot.h"
#include "Player.h"

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

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTexBf"));
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
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y))
	{
		// 아이템 UI를 눌렀다가 가져다 놨을 때 해당 마우스 위치의 슬롯이 비어있는지 판별 후 여부에 따른 처리
		if (Engine::InputDev()->Mouse_Pressing(DIM_LB))
		{
			m_bTooltipRender = false;
			m_bMove = true;
			
			m_pTransform->m_vInfo[INFO_POS].x = pt.x;
			m_pTransform->m_vInfo[INFO_POS].y = pt.y;
			WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

			if (m_ItemID.eItemType == ITEMTYPE_GENERALITEM || m_ItemID.eItemType == ITEMTYPE_EQUIPITEM)
			{
				// 슬롯 중에서 해당 UInumber 슬롯을 찾고 해당 슬롯 이미지 변경
				CGameObject* SlotObj = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIID_SLOTEQUIPMENT, m_UINumber);
				dynamic_cast<CUIequipmentslot*>(dynamic_cast<CTempUI*>(SlotObj))->Set_FindSlot(true);
			}
		}
		else
		{
			if (m_bMove)
			{
				m_bMove = false;

				UIOBJECTTTAG UIObjID;
				_uint        UINumber;

				CGameObject* ColliderSlotObj = Engine::UIManager()->Find_ColliderSlot();

				if (ColliderSlotObj == nullptr)
				{
					//// 해당 아이템 버리기
					//// 인벤토리에서 해당 아이템 아예 삭제 + 해당 개수 만큼 앞에다가 버림
					//CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
					//CInventory* Inventory = dynamic_cast<CInventory*>(pPlayer->Get_Component(COMPONENTTAG::INVENTORY, ID_DYNAMIC));

					//if(m_ItemID.iCount == 1)
					//{
					//	switch (m_ItemID.eItemType)
					//	{
					//	case ITEMTYPE_WEAPONITEM:
					//		pPlayer->Set_CurrentEquipRight(nullptr);
					//		pPlayer->Set_ItemEquipRight(false);
					//		break;
					//	case ITEMTYPE_GENERALITEM:
					//		pPlayer->Set_CurrentEquipLeft(nullptr);
					//		pPlayer->Set_ItemEquipLeft(false);
					//		break;
					//	}

					//	dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);
					//	Engine::UIManager()->Delete_FindItemUI(m_ItemID);
					//	Inventory->delete_FindItem(m_ItemID);
					//}
				 //   else
				 //   {
					//	m_ItemID.iCount -= 1;
                        m_pTransform->m_vInfo[INFO_POS].x = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].x;
						m_pTransform->m_vInfo[INFO_POS].y = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].y;
						WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
				 //       
					//	Inventory->delete_FindItem(m_ItemID);
					//}
				}
				else if (ColliderSlotObj != nullptr)
				{
					_bool bSucess = false;

					dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_UIObjID(UIObjID, UINumber);

					switch (UIObjID)
					{
					case Engine::UIID_SLOTBASIC:
						bSucess = true;
						break;
					case Engine::UIID_SLOTEMPTY:
						bSucess = true;
						break;
					case Engine::UIID_SLOTEQUIPMENT:
						if (m_UINumber == UINumber) {
							bSucess = true;
						}
						else {
							bSucess = false;
						}
						break;
					}

					if (bSucess)
					{
						dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);

						m_pTransform->m_vInfo[INFO_POS].x = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].x;
						m_pTransform->m_vInfo[INFO_POS].y = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].y;
						WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

						Set_Parent(ColliderSlotObj);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(this);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(false);


						// 장착 성공 시 해당 정보 인벤토리에 업데이트
						CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
						if (pPlayer) { // 위치 이동 및 이전 위치에는 정보 초기화 (해제 X)
							CInventory* Inventory = dynamic_cast<CInventory*>(pPlayer->Get_Component(COMPONENTTAG::INVENTORY, ID_DYNAMIC));
							Inventory->Switch_InvenItem(m_ItemID, UIObjID, UINumber); // : 이동한 아이템 아이디, 이동할 슬롯 타입 및 번호
						}

						if (UIObjID == Engine::UIID_SLOTEQUIPMENT)
						{
							// 장착한 슬롯이 장착 슬롯일 시 해당 값 만큼 플레이어 스탯에 업데이트




							dynamic_cast<CUIequipmentslot*>(Get_Parent())->Set_UseSlot(true); // 해당 슬롯을 사용함으로 써 플레이어 스탯 업데이트 상태이다.
						}
					}
					else
					{
						m_pTransform->m_vInfo[INFO_POS].x = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].x;
						m_pTransform->m_vInfo[INFO_POS].y = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].y;
						WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
					}
				}

				if (m_ItemID.eItemType == ITEMTYPE_GENERALITEM || m_ItemID.eItemType == ITEMTYPE_EQUIPITEM)
				{
					CGameObject* SlotObj = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIID_SLOTEQUIPMENT, m_UINumber);
					dynamic_cast<CUIequipmentslot*>(dynamic_cast<CTempUI*>(SlotObj))->Set_FindSlot(false);
				}
			}
		}

		if(!m_bMove)
		{
			m_bTooltipRender = true;

			m_fTooltipPosX = pt.x;
			m_fTooltipPosY = pt.y;
		}
	}
	else
	{
		m_bTooltipRender = false;
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
