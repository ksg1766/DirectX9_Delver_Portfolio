#include "..\..\Header\Inventory.h"

#include <Export_Utility.h>

CInventory::CInventory()
{

}

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CInventory::CInventory(const CInventory& rhs)
	: CComponent(rhs), m_mapKeySlot(rhs.m_mapKeySlot), m_mapItemSlot(rhs.m_mapItemSlot), m_vecInventory(rhs.m_vecInventory)
{
	for (_uint i = 0; i < KEYSLOT_END; i++) {
		m_bKeySlotEmpty[i] = rhs.m_bKeySlotEmpty[i];
	}

	for (_uint i = 0; i < ITEMSLOT_END; i++) {
		m_bItemSlotEmpty[i] = rhs.m_bItemSlotEmpty[i];
	}
}

CInventory::~CInventory()
{
	CInventory::Free();
}

void CInventory::Add_ItemObject(CGameObject* pGameObject)
{
	// 들어온 아이템 오브젝트 아이템 타입 및 아이디, 개수를 가져옴.
	ITEMTYPEID ItemType = dynamic_cast<CItem*>(pGameObject)->Get_ItemTag();

    // 이미 보유하고 있는 아이템인지 검사한다.
	// 키슬롯 검사
	for (auto& iter : m_mapKeySlot) {
		if (iter.second != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter.second)->Get_ItemTag();

			if (SlotItemType.eItemID == ItemType.eItemID)
			{
				// 같은 아이템이 존재할 시 해당 개수만큼 카운트 증가 후 들어온 아이템 삭제
				dynamic_cast<CItem*>(iter.second)->Add_ItemCount(ItemType.iCount);
				Safe_Release<CGameObject*>(pGameObject);
				return;
			}
		}
	}

	// 내부 인벤토리 검사
	for (auto& iter : m_vecInventory) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemID == ItemType.eItemID)
			{
				// 같은 아이템이 존재할 시 해당 개수만큼 카운트 증가 후 들어온 아이템 삭제
				dynamic_cast<CItem*>(iter)->Add_ItemCount(ItemType.iCount);
				Safe_Release<CGameObject*>(pGameObject);
				return;
			}
		}
	}

	// 보유하고 있지 않은 아이템이기에
	// 키 슬롯이 비어있으면 해당 슬롯에 들어가고
	if (m_bKeySlotEmpty[KEYSLOT_ONE] == false)
	{
		m_bKeySlotEmpty[KEYSLOT_ONE] = true;
		m_mapKeySlot[KEYSLOT_ONE] = pGameObject;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_TWO] == false)
	{
		m_bKeySlotEmpty[KEYSLOT_TWO] = true;
		m_mapKeySlot[KEYSLOT_TWO] = pGameObject;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_THREE] == false)
	{
		m_bKeySlotEmpty[KEYSLOT_THREE] = true;
		m_mapKeySlot[KEYSLOT_THREE] = pGameObject;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FOUR] == false)
	{
		m_bKeySlotEmpty[KEYSLOT_FOUR] = true;
		m_mapKeySlot[KEYSLOT_FOUR] = pGameObject;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FIVE] == false)
	{
		m_bKeySlotEmpty[KEYSLOT_FIVE] = true;
		m_mapKeySlot[KEYSLOT_FIVE] = pGameObject;
		return;
	}

	// 키 슬롯이 가득 차있을 시 내부 인벤에 넣는다.
	m_vecInventory.push_back(pGameObject);
}

void CInventory::delete_FindItem(ITEMTYPEID _itemId)
{
	// 해당 아이템의 아이디를 찾아서 삭제.
	// 순차적으로 키슬롯 검사
	if (m_bKeySlotEmpty[KEYSLOT_ONE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_ONE])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_ONE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_ONE]);
				m_bKeySlotEmpty[KEYSLOT_ONE] = false;
				return;
			}
		}
	}
	else if (m_bKeySlotEmpty[KEYSLOT_TWO] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_TWO])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_TWO])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_TWO]);
				m_bKeySlotEmpty[KEYSLOT_TWO] = false;
				return;
			}
		}
	}
	else if (m_bKeySlotEmpty[KEYSLOT_THREE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_THREE])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_THREE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_THREE]);
				m_bKeySlotEmpty[KEYSLOT_THREE] = false;
				return;
			}
		}
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FOUR] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FOUR])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FOUR])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_FOUR]);
				m_bKeySlotEmpty[KEYSLOT_FOUR] = false;
				return;
			}
		}
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FIVE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FIVE])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FIVE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_FIVE]);
				m_bKeySlotEmpty[KEYSLOT_FIVE] = false;
				return;
			}
		}
	}

	// 내부 인벤토리 검사
	for (auto& iter : m_vecInventory) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemType == _itemId.eItemType)
			{
				// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
				if (SlotItemType.iCount > _itemId.iCount)
				{
					dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FIVE])->Remove_ItemCount(_itemId.iCount);
					return;
				}
				else
				{
					Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_FIVE]);
					return;
				}
			}
		}
	}
}

HRESULT CInventory::Ready_Inventory()
{
	// m_bKeySlotEmpty 초기화
	for (_uint i = 0; i < KEYSLOT_END; i++) {
		m_bKeySlotEmpty[i] = false;
	}

	// m_bItemSlotEmpty 초기화
	for (_uint i = 0; i < ITEMSLOT_END; i++) {
		m_bItemSlotEmpty[i] = false;
	}

	return S_OK;
}

void CInventory::LateUpdate_Component()
{
	
}

CInventory* CInventory::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventory* pInstance = new CInventory(pGraphicDev);

	if (FAILED(pInstance->Ready_Inventory()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Inventory Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CInventory::Clone(void)
{
	return new CInventory(*this);
}

void CInventory::Free()
{
	if (false == m_bClone)
	{

		for (auto& Mapiter : m_mapKeySlot) {
			Safe_Release(Mapiter.second);
		}

		for (auto& Mapiter : m_mapItemSlot) {
			Safe_Release(Mapiter.second);
		}

		for_each(m_vecInventory.begin(), m_vecInventory.end(), CDeleteObj());
		m_vecInventory.clear();
	}

	CComponent::Free();
}
