#include "stdafx.h"
#include "..\Header\UIequipmentslot.h"
#include "Player.h"
#include "Export_Function.h"
#include "UIitem.h"

CUIequipmentslot::CUIequipmentslot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIequipmentslot::~CUIequipmentslot()
{

}

HRESULT CUIequipmentslot::Ready_Object()
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

_int CUIequipmentslot::Update_Object(const _float & fTimeDelta)
{
	CPlayer*    pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	if (pPlayer != nullptr) {
		CInventory* pInventory = dynamic_cast<CInventory*>(pPlayer->Get_Component(COMPONENTTAG::INVENTORY, ID_DYNAMIC));

		// 다음에 장착해야하는 자식이 존재할 시 장착 해제 및 다음 자식 장착
		if (m_pNextChild != nullptr && !m_bNextItem) {
			m_bChildExit = true;
			m_bNextItem  = true;
		}

		if (m_bChildEntrance && !m_bEntrance) // 해당 아이템 슬롯을 사용할 시 효과 적용
		{
			m_bEntrance = true;

			ITEMTYPEID   eItemId = dynamic_cast<CUIitem*>(m_pChild)->Get_ItemTag();
			CGameObject* pGameObject = pInventory->Get_IDItem(eItemId.eItemID);

			if (pGameObject != nullptr) {
				CBasicStat* pStat = dynamic_cast<CItem*>(pGameObject)->Get_ItemStat();
				if (pStat != nullptr)
				{
					pPlayer->Get_Stat()->Get_Stat()->iArmorMax += pStat->Get_Stat()->iArmorMax;
					pPlayer->Get_Stat()->Get_Stat()->iArmorMin += pStat->Get_Stat()->iArmorMin;

					if (eItemId.eItemType == ITEMTYPE_GENERALITEM) {
						pPlayer->Set_PrevEquipLeft(pPlayer->Get_CurrentEquipLeft());
						pPlayer->Set_CurrentEquipLeft(pGameObject);
						pPlayer->Set_ItemEquipLeft(true);
					}
				}
			}
		}
		else if (m_bChildExit && m_bEntrance) // 해당 아이템 슬롯을 사용했다가 비었을 시 효과 적용 해제
		{
			ITEMTYPEID   eItemId = dynamic_cast<CUIitem*>(m_pBeforeChild)->Get_ItemTag();
			CGameObject* pGameObject = pInventory->Get_IDItem(eItemId.eItemID);

			if (pGameObject != nullptr && pGameObject->Get_ObjectTag() != OBJECTTAG::BACKGROUND) {
				CBasicStat* pStat = dynamic_cast<CItem*>(pGameObject)->Get_ItemStat();
				if (pStat != nullptr)
				{
					pPlayer->Get_Stat()->Get_Stat()->iArmorMax -= pStat->Get_Stat()->iArmorMax;
					pPlayer->Get_Stat()->Get_Stat()->iArmorMin -= pStat->Get_Stat()->iArmorMin;

					if (pPlayer->Get_CurrentEquipLeft() != nullptr && dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipLeft())->Get_ItemTag().eItemID == eItemId.eItemID) {
						pPlayer->Set_PrevEquipLeft(pPlayer->Get_CurrentEquipLeft());
						pPlayer->Set_CurrentEquipLeft(nullptr);
						pPlayer->Set_ItemEquipLeft(false);
					}
				}
			}

			if (m_eThrowitem != ITEMID::ITEMID_END) {
				pInventory->delete_FindItem(eItemId);
				m_eThrowitem = ITEMID::ITEMID_END;
			}

			if (m_bNextItem)
			{
				m_pChild = m_pNextChild;
				m_bChildEntrance = true;

				m_pNextChild = nullptr;
				m_bNextItem = false;
			}

			m_bEntrance = false;
		}
	}
	
	if (m_IsDead)
		return 0;

	if (m_bSetup) {
		m_bSetup = false;

		switch (m_UINumber)
		{
		case 0:
			m_fCurrentImage = 2;
			break;
		case 1:
			m_fCurrentImage = 11;
			break;
		case 2:
			m_fCurrentImage = 5;
			break;
		case 3:
			m_fCurrentImage = 17;
			break;
		case 4:
			m_fCurrentImage = 8;
			break;
		case 5:
			m_fCurrentImage = 14;
			break;
		}
	}

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIequipmentslot::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUIequipmentslot::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIequipmentslot::Add_Component(void)
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

void CUIequipmentslot::Key_Input(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y))
	{
		Engine::UIManager()->Set_ColliderSlot(m_UIObjID, m_UINumber, true);

		if (!m_bFind)
		{
			switch (m_UINumber)
			{
			case 0:
				m_fCurrentImage = 3;
				break;
			case 1:
				m_fCurrentImage = 12;
				break;
			case 2:
				m_fCurrentImage = 6;
				break;
			case 3:
				m_fCurrentImage = 18;
				break;
			case 4:
				m_fCurrentImage = 9;
				break;
			case 5:
				m_fCurrentImage = 15;
				break;
			}
		}
	}
	else
	{
		Engine::UIManager()->Set_ColliderSlot(m_UIObjID, m_UINumber, false);

		if (!m_bFind)
		{
			switch (m_UINumber)
			{
			case 0:
				m_fCurrentImage = 2;
				break;
			case 1:
				m_fCurrentImage = 11;
				break;
			case 2:
				m_fCurrentImage = 5;
				break;
			case 3:
				m_fCurrentImage = 17;
				break;
			case 4:
				m_fCurrentImage = 8;
				break;
			case 5:
				m_fCurrentImage = 14;
				break;
			}
		}
	}
}

CUIequipmentslot* CUIequipmentslot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIequipmentslot*	pInstance = new CUIequipmentslot(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUIequipmentslot::Free()
{
	CTempUI::Free();
}
