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
	//CInventory::Free();
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
				
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				while (iter != ChildTransform.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*iter)->Get_Host());

					if (pItem != nullptr)
						iter = ChildTransform.erase(iter);
					else
						++iter;
				}


				return;
			}
		}
	}

	// 장착 슬롯 검사
	for (auto& iter : m_mapItemSlot) {
		if (iter.second != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter.second)->Get_ItemTag();

			if (SlotItemType.eItemType == ItemType.eItemType)
			{
				// 같은 아이템이 존재할 시 해당 개수만큼 카운트 증가 후 들어온 아이템 삭제
				dynamic_cast<CItem*>(iter.second)->Add_ItemCount(ItemType.iCount);
				
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				while (iter != ChildTransform.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*iter)->Get_Host());

					if (pItem != nullptr)
						iter = ChildTransform.erase(iter);
					else
						++iter;
				}


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
				
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				while (iter != ChildTransform.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*iter)->Get_Host());

					if (pItem != nullptr)
						iter = ChildTransform.erase(iter);
					else
						++iter;
				}


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
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				while (iter != ChildTransform.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*iter)->Get_Host());

					if (pItem != nullptr)
						iter = ChildTransform.erase(iter);
					else
						++iter;
				}

				map<INVENKEYSLOT, CGameObject*>::iterator Mapiter = m_mapKeySlot.begin();

				while (Mapiter != m_mapKeySlot.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*Mapiter).second);

					if (pItem != nullptr)
						Mapiter = m_mapKeySlot.erase(Mapiter);
					else
						++Mapiter;
				}

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
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				while (iter != ChildTransform.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*iter)->Get_Host());

					if (pItem != nullptr)
						iter = ChildTransform.erase(iter);
					else
						++iter;
				}

				map<INVENKEYSLOT, CGameObject*>::iterator Mapiter = m_mapKeySlot.begin();

				while (Mapiter != m_mapKeySlot.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*Mapiter).second);

					if (pItem != nullptr)
						Mapiter = m_mapKeySlot.erase(Mapiter);
					else
						++Mapiter;
				}


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
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				while (iter != ChildTransform.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*iter)->Get_Host());

					if (pItem != nullptr)
						iter = ChildTransform.erase(iter);
					else
						++iter;
				}

				map<INVENKEYSLOT, CGameObject*>::iterator Mapiter = m_mapKeySlot.begin();

				while (Mapiter != m_mapKeySlot.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*Mapiter).second);

					if (pItem != nullptr)
						Mapiter = m_mapKeySlot.erase(Mapiter);
					else
						++Mapiter;
				}


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
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				while (iter != ChildTransform.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*iter)->Get_Host());

					if (pItem != nullptr)
						iter = ChildTransform.erase(iter);
					else
						++iter;
				}

				map<INVENKEYSLOT, CGameObject*>::iterator Mapiter = m_mapKeySlot.begin();

				while (Mapiter != m_mapKeySlot.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*Mapiter).second);

					if (pItem != nullptr)
						Mapiter = m_mapKeySlot.erase(Mapiter);
					else
						++Mapiter;
				}


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
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				while (iter != ChildTransform.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*iter)->Get_Host());

					if (pItem != nullptr)
						iter = ChildTransform.erase(iter);
					else
						++iter;
				}

				map<INVENKEYSLOT, CGameObject*>::iterator Mapiter = m_mapKeySlot.begin();

				while (Mapiter != m_mapKeySlot.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*Mapiter).second);

					if (pItem != nullptr)
						Mapiter = m_mapKeySlot.erase(Mapiter);
					else
						++Mapiter;
				}


				m_bKeySlotEmpty[KEYSLOT_FIVE] = false;
				return;
			}
		}
	}

	// 순차적으로 아이템 슬롯 검사
	if (m_bItemSlotEmpty[ITEMSLOT_HELMET] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_HELMET])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_HELMET])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				map<INVENITEMSLOT, CGameObject*>::iterator Mapiter = m_mapItemSlot.begin();

				while (Mapiter != m_mapItemSlot.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*Mapiter).second);

					if (pItem != nullptr)
						Mapiter = m_mapItemSlot.erase(Mapiter);
					else
						++Mapiter;
				}

				m_bItemSlotEmpty[ITEMSLOT_HELMET] = false;
				return;
			}
		}
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_HAND] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_HAND])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_HAND])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				while (iter != ChildTransform.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*iter)->Get_Host());

					if (pItem != nullptr)
						iter = ChildTransform.erase(iter);
					else
						++iter;
				}

				map<INVENITEMSLOT, CGameObject*>::iterator Mapiter = m_mapItemSlot.begin();

				while (Mapiter != m_mapItemSlot.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*Mapiter).second);

					if (pItem != nullptr)
						Mapiter = m_mapItemSlot.erase(Mapiter);
					else
						++Mapiter;
				}

				m_bItemSlotEmpty[ITEMSLOT_HAND] = false;
				return;
			}
		}
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_ARMOR] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_ARMOR])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_ARMOR])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				map<INVENITEMSLOT, CGameObject*>::iterator Mapiter = m_mapItemSlot.begin();

				while (Mapiter != m_mapItemSlot.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*Mapiter).second);

					if (pItem != nullptr)
						Mapiter = m_mapItemSlot.erase(Mapiter);
					else
						++Mapiter;
				}

				m_bItemSlotEmpty[ITEMSLOT_ARMOR] = false;
				return;
			}
		}
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_RING] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_RING])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_RING])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				map<INVENITEMSLOT, CGameObject*>::iterator Mapiter = m_mapItemSlot.begin();

				while (Mapiter != m_mapItemSlot.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*Mapiter).second);

					if (pItem != nullptr)
						Mapiter = m_mapItemSlot.erase(Mapiter);
					else
						++Mapiter;
				}


				m_bItemSlotEmpty[ITEMSLOT_RING] = false;
				return;
			}
		}
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_PANTS] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_PANTS])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_PANTS])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				map<INVENITEMSLOT, CGameObject*>::iterator Mapiter = m_mapItemSlot.begin();

				while (Mapiter != m_mapItemSlot.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*Mapiter).second);

					if (pItem != nullptr)
						Mapiter = m_mapItemSlot.erase(Mapiter);
					else
						++Mapiter;
				}

				m_bItemSlotEmpty[ITEMSLOT_PANTS] = false;
				return;
			}
		}
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_NECKLACE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_NECKLACE])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_NECKLACE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				map<INVENITEMSLOT, CGameObject*>::iterator Mapiter = m_mapItemSlot.begin();

				while (Mapiter != m_mapItemSlot.end())
				{
					CItem* pItem = dynamic_cast<CItem*>((*Mapiter).second);

					if (pItem != nullptr)
						Mapiter = m_mapItemSlot.erase(Mapiter);
					else
						++Mapiter;
				}

				m_mapItemSlot[ITEMSLOT_NECKLACE] = false;
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
					dynamic_cast<CItem*>(iter)->Remove_ItemCount(_itemId.iCount);
					return;
				}
				else
				{
					vector<CGameObject*>::iterator iter = m_vecInventory.begin();

					while (iter != m_vecInventory.end())
					{
						CItem* pItem = dynamic_cast<CItem*>(*iter);

						if (pItem != nullptr)
							iter = m_vecInventory.erase(iter);
						else
							++iter;
					}

					return;
				}
			}
		}
	}
}

void CInventory::Switch_InvenItem(ITEMTYPEID _itemId, UIOBJECTTTAG _slotId, _uint _UINumber)
{
	m_bSwitch = true;

	// 해당 아이템의 아이디를 찾아서 이동
	if (m_bKeySlotEmpty[KEYSLOT_ONE] == true) {
		Switch_Keyslot(KEYSLOT_ONE, _itemId, _slotId, _UINumber);
	}
	else if (m_bKeySlotEmpty[KEYSLOT_TWO] == true) {
		Switch_Keyslot(KEYSLOT_TWO, _itemId, _slotId, _UINumber);
	}
	else if (m_bKeySlotEmpty[KEYSLOT_THREE] == true) {
		Switch_Keyslot(KEYSLOT_THREE, _itemId, _slotId, _UINumber);
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FOUR] == true) {
		Switch_Keyslot(KEYSLOT_FOUR, _itemId, _slotId, _UINumber);
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FIVE] == true) {
		Switch_Keyslot(KEYSLOT_FIVE, _itemId, _slotId, _UINumber);
	}

	if (!m_bSwitch)
		return;

	// 아이템 슬롯 검사
	if (m_bItemSlotEmpty[ITEMSLOT_HELMET] == true) {
		Switch_Itemslot(ITEMSLOT_HELMET, _itemId, _slotId, _UINumber);
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_HAND] == true) {
		Switch_Itemslot(ITEMSLOT_HAND, _itemId, _slotId, _UINumber);
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_ARMOR] == true) {
		Switch_Itemslot(ITEMSLOT_ARMOR, _itemId, _slotId, _UINumber);
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_RING] == true) {
		Switch_Itemslot(ITEMSLOT_RING, _itemId, _slotId, _UINumber);
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_PANTS] == true) {
		Switch_Itemslot(ITEMSLOT_PANTS, _itemId, _slotId, _UINumber);
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_NECKLACE] == true) {
		Switch_Itemslot(ITEMSLOT_NECKLACE, _itemId, _slotId, _UINumber);
	}

	if (!m_bSwitch)
		return;

	// 내부 인벤토리 검사
	for (auto& iter : m_vecInventory) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemType == _itemId.eItemType)
			{
				switch (_slotId)
				{
				case Engine::UIID_SLOTBASIC: // 내부 인벤 -> 키 슬롯 이동
					switch (_UINumber)       // 해당 타입의 슬롯 번호
					{
					case 0:
						m_mapKeySlot[KEYSLOT_ONE] = iter;
						m_bKeySlotEmpty[KEYSLOT_ONE] = true;
						break;
					case 1:
						m_mapKeySlot[KEYSLOT_TWO] = iter;
						m_bKeySlotEmpty[KEYSLOT_TWO] = true;
						break;
					case 2:
						m_mapKeySlot[KEYSLOT_THREE] = iter;
						m_bKeySlotEmpty[KEYSLOT_THREE] = true;
						break;
					case 3:
						m_mapKeySlot[KEYSLOT_FOUR] = iter;
						m_bKeySlotEmpty[KEYSLOT_FOUR] = true;
						break;
					case 4:
						m_mapKeySlot[KEYSLOT_FIVE] = iter;
						m_bKeySlotEmpty[KEYSLOT_FIVE] = true;
						break;
					}
					break;
				case Engine::UIID_SLOTEQUIPMENT: // 내부 인벤 -> 장착 슬롯 이동
					switch (_UINumber)       // 해당 타입의 슬롯 번호
					{
					case 0:
						m_mapItemSlot[ITEMSLOT_HELMET] = iter;
						m_bItemSlotEmpty[ITEMSLOT_HELMET] = true;
						break;
					case 1:
						m_mapItemSlot[ITEMSLOT_HAND] = iter;
						m_bItemSlotEmpty[ITEMSLOT_HAND] = true;
						break;
					case 2:
						m_mapItemSlot[ITEMSLOT_ARMOR] = iter;
						m_bItemSlotEmpty[ITEMSLOT_ARMOR] = true;
						break;
					case 3:
						m_mapItemSlot[ITEMSLOT_RING] = iter;
						m_bItemSlotEmpty[ITEMSLOT_RING] = true;
						break;
					case 4:
						m_mapItemSlot[ITEMSLOT_PANTS] = iter;
						m_bItemSlotEmpty[ITEMSLOT_PANTS] = true;
						break;
					case 5:
						m_mapItemSlot[ITEMSLOT_NECKLACE] = iter;
						m_bItemSlotEmpty[ITEMSLOT_NECKLACE] = true;
						break;
					}
					break;
				}

				iter = nullptr;
				m_bSwitch = false;
			}
		}
	}
}

void CInventory::Switch_Keyslot(INVENKEYSLOT _key, ITEMTYPEID _itemId, UIOBJECTTTAG _slotId, _uint _UINumber)
{
	if (m_mapKeySlot[_key] == nullptr)
		return;

	// 해당 키 슬롯 아이템 태그를 가져온다.
	ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[_key])->Get_ItemTag();
	// 같은 아이템을 찾았을 시 
	if (SlotItemType.eItemType == _itemId.eItemType)
	{
		switch (_slotId)             // 이동하려는 슬롯 타입
		{
		case Engine::UIID_SLOTBASIC: // 키 슬롯
			switch (_UINumber)       // 해당 타입의 슬롯 번호
			{
			case 0:
				if (_UINumber == 0)
					return;
				m_mapKeySlot[KEYSLOT_ONE] = m_mapKeySlot[_key];
				m_bKeySlotEmpty[KEYSLOT_ONE] = true;
				break;
			case 1:
				if (_UINumber == 1)
					return;
				m_mapKeySlot[KEYSLOT_TWO] = m_mapKeySlot[_key];
				m_bKeySlotEmpty[KEYSLOT_TWO] = true;
				break;
			case 2:
				if (_UINumber == 2)
					return;
				m_mapKeySlot[KEYSLOT_THREE] = m_mapKeySlot[_key];
				m_bKeySlotEmpty[KEYSLOT_THREE] = true;
				break;
			case 3:
				if (_UINumber == 3)
					return;
				m_mapKeySlot[KEYSLOT_FOUR] = m_mapKeySlot[_key];
				m_bKeySlotEmpty[KEYSLOT_FOUR] = true;
				break;
			case 4:
				if (_UINumber == 4)
					return;
				m_mapKeySlot[KEYSLOT_FIVE] = m_mapKeySlot[_key];
				m_bKeySlotEmpty[KEYSLOT_FIVE] = true;
				break;
			}
			break;

		case Engine::UIID_SLOTEMPTY:     // 내부 슬롯
			m_vecInventory.push_back(m_mapKeySlot[_key]);
			break;

		case Engine::UIID_SLOTEQUIPMENT: // 장착 슬롯
			switch (_UINumber)           // 해당 타입의 슬롯 번호
			{
			case 0:
				m_mapItemSlot[ITEMSLOT_HELMET] = m_mapKeySlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_HELMET] = true;
				break;
			case 1:
				m_mapItemSlot[ITEMSLOT_HAND] = m_mapKeySlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_HAND] = true;
				break;
			case 2:
				m_mapItemSlot[ITEMSLOT_ARMOR] = m_mapKeySlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_ARMOR] = true;
				break;
			case 3:
				m_mapItemSlot[ITEMSLOT_RING] = m_mapKeySlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_RING] = true;
				break;
			case 4:
				m_mapItemSlot[ITEMSLOT_PANTS] = m_mapKeySlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_PANTS] = true;
				break;
			case 5:
				m_mapItemSlot[ITEMSLOT_NECKLACE] = m_mapKeySlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_NECKLACE] = true;
				break;
			}
			break;
		}

		m_bKeySlotEmpty[_key] = false;
		m_mapKeySlot[_key]    = nullptr;
		m_bSwitch = false;
	}
}

void CInventory::Switch_Itemslot(INVENITEMSLOT _key, ITEMTYPEID _itemId, UIOBJECTTTAG _slotId, _uint _UINumber)
{
	if (m_mapItemSlot[_key] == nullptr)
		return;

	// 해당 키 슬롯 아이템 태그를 가져온다.
	ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[_key])->Get_ItemTag();
	// 같은 아이템을 찾았을 시 
	if (SlotItemType.eItemType == _itemId.eItemType)
	{
		switch (_slotId)             // 이동하려는 슬롯 타입
		{
		case Engine::UIID_SLOTBASIC: // 키 슬롯
			switch (_UINumber)       // 해당 타입의 슬롯 번호
			{
			case 0:
				m_mapKeySlot[KEYSLOT_ONE] = m_mapItemSlot[_key];
				m_bKeySlotEmpty[KEYSLOT_ONE] = true;
				break;
			case 1:
				m_mapKeySlot[KEYSLOT_TWO] = m_mapItemSlot[_key];
				m_bKeySlotEmpty[KEYSLOT_TWO] = true;
				break;
			case 2:
				m_mapKeySlot[KEYSLOT_THREE] = m_mapItemSlot[_key];
				m_bKeySlotEmpty[KEYSLOT_THREE] = true;
				break;
			case 3:
				m_mapKeySlot[KEYSLOT_FOUR] = m_mapItemSlot[_key];
				m_bKeySlotEmpty[KEYSLOT_FOUR] = true;
				break;
			case 4:
				m_mapKeySlot[KEYSLOT_FIVE] = m_mapItemSlot[_key];
				m_bKeySlotEmpty[KEYSLOT_FIVE] = true;
				break;
			}
			break;

		case Engine::UIID_SLOTEMPTY:     // 내부 슬롯
			m_vecInventory.push_back(m_mapItemSlot[_key]);
			break;

		case Engine::UIID_SLOTEQUIPMENT: // 장착 슬롯
			switch (_UINumber)           // 해당 타입의 슬롯 번호
			{
			case 0:
				if (_UINumber == 0)
					return;
				m_mapItemSlot[ITEMSLOT_HELMET] = m_mapItemSlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_HELMET] = true;
				break;
			case 1:
				if (_UINumber == 1)
					return;
				m_mapItemSlot[ITEMSLOT_HAND] = m_mapItemSlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_HAND] = true;
				break;
			case 2:
				if (_UINumber == 2)
					return;
				m_mapItemSlot[ITEMSLOT_ARMOR] = m_mapItemSlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_ARMOR] = true;
				break;
			case 3:
				if (_UINumber == 3)
					return;
				m_mapItemSlot[ITEMSLOT_RING] = m_mapItemSlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_RING] = true;
				break;
			case 4:
				if (_UINumber == 4)
					return;
				m_mapItemSlot[ITEMSLOT_PANTS] = m_mapItemSlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_PANTS] = true;
				break;
			case 5:
				if (_UINumber == 5)
					return;
				m_mapItemSlot[ITEMSLOT_NECKLACE] = m_mapItemSlot[_key];
				m_bItemSlotEmpty[ITEMSLOT_NECKLACE] = true;
				break;
			}
			break;
		}

		m_bItemSlotEmpty[_key] = false;
		m_mapItemSlot[_key] = nullptr;
		m_bSwitch = false;
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
