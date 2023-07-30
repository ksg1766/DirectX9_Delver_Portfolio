#include "..\..\Header\Inventory.h"
#include "Export_Function.h"

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

void CInventory::Add_ItemObject(CGameObject* pGameObject)
{
	// 들어온 아이템 오브젝트 아이템 타입 및 아이디, 개수를 가져옴.
	ITEMTYPEID ItemType = dynamic_cast<CItem*>(pGameObject)->Get_ItemTag();

    // 이미 보유하고 있는 아이템인지 검사한다.
	//
	// 키슬롯 검사
	for (auto& iter : m_mapKeySlot) {
		if (iter.second != nullptr) {
			// 해당 슬롯에 존재하는 아이템 타입을 가져온다.
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter.second)->Get_ItemTag();

			// 슬롯에 있는 아이템 타입과 들어온 아이템 타입이 같다면
			if (SlotItemType.eItemID == ItemType.eItemID)
			{
				// 해당 슬롯의 아이템 카운트를 올려주고
				dynamic_cast<CItem*>(iter.second)->Add_ItemCount(ItemType.iCount);
				dynamic_cast<CItem*>(pGameObject)->m_pTransform;

				CTransform* pPlayerTransform        = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				ChildTransform.erase(remove_if(ChildTransform.begin(), ChildTransform.end(),
					[&](CTransform* pTramsform)
					{
						if (pTramsform == dynamic_cast<CItem*>(pGameObject)->m_pTransform)
							return true;

						return false;
					}
				),
					ChildTransform.end());

				dynamic_cast<CItem*>(pGameObject)->m_pTransform = nullptr;
				EventManager()->DeleteObject(pGameObject);
				return;
			}
		}
	}

	// 장착 슬롯 검사
	for (auto& iter : m_mapItemSlot) {
		if (iter.second != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter.second)->Get_ItemTag();

			if (SlotItemType.eItemID == ItemType.eItemID)
			{
				dynamic_cast<CItem*>(iter.second)->Add_ItemCount(ItemType.iCount);
				dynamic_cast<CItem*>(pGameObject)->m_pTransform;

				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				ChildTransform.erase(remove_if(ChildTransform.begin(), ChildTransform.end(),
					[&](CTransform* pTramsform)
					{
						if (pTramsform == dynamic_cast<CItem*>(pGameObject)->m_pTransform)
							return true;

						return false;
					}
				),
					ChildTransform.end());

				dynamic_cast<CItem*>(pGameObject)->m_pTransform = nullptr;
				EventManager()->DeleteObject(pGameObject);
				return;
			}
		}
	}

	// 내부 인벤토리 검사
	for (auto& iter = m_vecInventory.begin(); iter != m_vecInventory.end();) {
		if (*iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(*iter)->Get_ItemTag();

			if (SlotItemType.eItemID == ItemType.eItemID)
			{
				dynamic_cast<CItem*>(*iter)->Add_ItemCount(ItemType.iCount);
				dynamic_cast<CItem*>(pGameObject)->m_pTransform;

				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				ChildTransform.erase(remove_if(ChildTransform.begin(), ChildTransform.end(),
					[&](CTransform* pTramsform)
					{
						if (pTramsform == dynamic_cast<CItem*>(pGameObject)->m_pTransform)
							return true;

						return false;
					}
				),
					ChildTransform.end());

				dynamic_cast<CItem*>(pGameObject)->m_pTransform = nullptr;
				EventManager()->DeleteObject(pGameObject);
				return;
			}
			else
				++iter;
		}
		else
			++iter;
	}

	// 보유하고 있지 않은 아이템이기에
	// 키 슬롯이 비어있으면 해당 슬롯에 들어가고
	if (m_bKeySlotEmpty[KEYSLOT_ONE] == false)
	{
		m_mapKeySlot[KEYSLOT_ONE] = pGameObject;
		m_bKeySlotEmpty[KEYSLOT_ONE] = true;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_TWO] == false)
	{
		m_mapKeySlot[KEYSLOT_TWO] = pGameObject;
		m_bKeySlotEmpty[KEYSLOT_TWO] = true;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_THREE] == false)
	{
		m_mapKeySlot[KEYSLOT_THREE] = pGameObject;
		m_bKeySlotEmpty[KEYSLOT_THREE] = true;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FOUR] == false)
	{
		m_mapKeySlot[KEYSLOT_FOUR] = pGameObject;
		m_bKeySlotEmpty[KEYSLOT_FOUR] = true;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FIVE] == false)
	{
		m_mapKeySlot[KEYSLOT_FIVE] = pGameObject;
		m_bKeySlotEmpty[KEYSLOT_FIVE] = true;
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
	
		if (SlotItemType.eItemID == _itemId.eItemID)
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

				for (auto& iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapKeySlot.begin(); iter != m_mapKeySlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapKeySlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;
	

				m_bKeySlotEmpty[KEYSLOT_ONE] = false;
				return;
			}
		}
	}
	if (m_bKeySlotEmpty[KEYSLOT_TWO] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_TWO])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
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

				for (auto& iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}


				for (auto& iter = m_mapKeySlot.begin(); iter != m_mapKeySlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapKeySlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_bKeySlotEmpty[KEYSLOT_TWO] = false;
				return;
			}
		}
	}
	if (m_bKeySlotEmpty[KEYSLOT_THREE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_THREE])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
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

				for (auto& iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapKeySlot.begin(); iter != m_mapKeySlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapKeySlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;



				m_bKeySlotEmpty[KEYSLOT_THREE] = false;
				return;
			}
		}
	}
	if (m_bKeySlotEmpty[KEYSLOT_FOUR] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FOUR])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
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


				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							iter = ChildTransform.erase(iter);
						}
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapKeySlot.begin(); iter != m_mapKeySlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapKeySlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_bKeySlotEmpty[KEYSLOT_FOUR] = false;
				return;
			}
		}
	}
	if (m_bKeySlotEmpty[KEYSLOT_FIVE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FIVE])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
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

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}


				for (auto& iter = m_mapKeySlot.begin(); iter != m_mapKeySlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapKeySlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_bKeySlotEmpty[KEYSLOT_FIVE] = false;
				return;
			}
		}
	}

	// 순차적으로 아이템 슬롯 검사
	if (m_bItemSlotEmpty[ITEMSLOT_HELMET] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_HELMET])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_HELMET])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;



				m_bItemSlotEmpty[ITEMSLOT_HELMET] = false;
				return;
			}
		}
	}
	if (m_bItemSlotEmpty[ITEMSLOT_HAND] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_HAND])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
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

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_bItemSlotEmpty[ITEMSLOT_HAND] = false;
				return;
			}
		}
	}
	if (m_bItemSlotEmpty[ITEMSLOT_ARMOR] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_ARMOR])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_ARMOR])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_bItemSlotEmpty[ITEMSLOT_ARMOR] = false;
				return;
			}
		}
	}
	if (m_bItemSlotEmpty[ITEMSLOT_RING] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_RING])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_RING])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
				{
					if (iter->second != nullptr)
					{
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					}
					else
						++iter;
				}


				m_bItemSlotEmpty[ITEMSLOT_RING] = false;
				return;
			}
		}
	}
	if (m_bItemSlotEmpty[ITEMSLOT_PANTS] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_PANTS])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_PANTS])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{

				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;

				m_bItemSlotEmpty[ITEMSLOT_PANTS] = false;
				return;
			}
		}
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_NECKLACE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_NECKLACE])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_NECKLACE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_mapItemSlot[ITEMSLOT_NECKLACE] = false;
				return;
			}
		}
	}

	// 내부 인벤토리 검사
	for (auto& iter : m_vecInventory) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemID == _itemId.eItemID)
			{
				// 같은 아이템이 존재하고 들어온 개수보다 많을 시 카운트만 감소 / 같은 개수일 시 삭제
				if (SlotItemType.iCount > _itemId.iCount)
				{
					dynamic_cast<CItem*>(iter)->Remove_ItemCount(_itemId.iCount);
					return;
				}
				else
				{
					CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

					vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
					vector<CTransform*>::iterator iter = ChildTransform.begin();

					for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
					{
						if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
						{
							if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
								iter = ChildTransform.erase(iter);
							else
								++iter;
						}
						else
							++iter;
					}

					for (auto& iter = m_vecInventory.begin(); iter != m_vecInventory.end();)
					{
						if (*iter != nullptr)
						{
							if (dynamic_cast<CItem*>(*iter)->Get_ItemTag().eItemID == SlotItemType.eItemID)
							{
								m_vDead.push_back(*iter);
								iter = m_vecInventory.erase(iter);
							}
							else
								++iter;
						}
						else
							++iter;
					}

					return;
				}
			}
		}
	}
}

CGameObject* CInventory::Get_IDItem(ITEMID _eID)
{
	// 키 슬롯 검사
	for (auto& iter : m_mapKeySlot) {
		if (iter.second != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter.second)->Get_ItemTag();

			if (SlotItemType.eItemID == _eID)
			{
				return iter.second;
			}
		}
	}

	// 장착 슬롯 검사
	for (auto& iter : m_mapItemSlot) {
		if (iter.second != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter.second)->Get_ItemTag();

			if (SlotItemType.eItemID == _eID)
			{
				return iter.second;
			}
		}
	}

	// 내부 인벤토리 검사
	for (auto& iter : m_vecInventory) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemID == _eID)
			{
				return iter;
			}
		}
	}
}

void CInventory::GoSwitch_InvenItem(ITEMTYPEID  _ItemID, UIOBJECTTTAG _StartslotId, _uint _StartUINumber, UIOBJECTTTAG _EndslotId, _uint _EndUINumber)
{
	// 시작 위치에서 해당 아이템을 찾아오고 해당 공간은 지운다.
	CGameObject* pFindItemObject = nullptr;

	switch (_StartslotId)
	{
	case Engine::UIID_SLOTBASIC:
		pFindItemObject = Find_KeySlotItem(_StartUINumber);
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		pFindItemObject = Find_ItemSlotItem(_StartUINumber);
		break;

	case Engine::UIID_SLOTEMPTY:    
		pFindItemObject = Find_InvenSlotItem(_ItemID);
		break;
	}

	if (pFindItemObject == nullptr)
		return;

	// 찾은 아이템을 이동한다.
	switch (_EndslotId)
	{
	case Engine::UIID_SLOTBASIC:
		m_mapKeySlot[(INVENKEYSLOT)_EndUINumber] = pFindItemObject;
		m_bKeySlotEmpty[_EndUINumber] = true;
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		m_mapItemSlot[(INVENITEMSLOT)_EndUINumber] = pFindItemObject;
		m_bItemSlotEmpty[_EndUINumber] = true;
		break;

	case Engine::UIID_SLOTEMPTY:
		m_vecInventory.push_back(pFindItemObject);
		break;
	}
}

void CInventory::ExSwitch_InvenItem(ITEMTYPEID  _StartItemID, UIOBJECTTTAG _StartslotId, _uint _StartUINumber, ITEMTYPEID  _EndItemID, UIOBJECTTTAG _EndslotId, _uint _EndUINumber)
{
	// 서로 스위칭할 아이템들을 찾아온다.
	CGameObject* pStartItemObject = nullptr;
	CGameObject* pEndItemObject = nullptr;

	switch (_StartslotId)
	{
	case Engine::UIID_SLOTBASIC:
		pStartItemObject = Find_KeySlotItem(_StartUINumber);
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		pStartItemObject = Find_ItemSlotItem(_StartUINumber);
		break;

	case Engine::UIID_SLOTEMPTY:
		pStartItemObject = Find_InvenSlotItem(_StartItemID);
		break;
	}

	if (pStartItemObject == nullptr)
		return;

	switch (_EndslotId)
	{
	case Engine::UIID_SLOTBASIC:
		pEndItemObject = Find_KeySlotItem(_EndUINumber);
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		pEndItemObject = Find_ItemSlotItem(_EndUINumber);
		break;

	case Engine::UIID_SLOTEMPTY:
		pEndItemObject = Find_InvenSlotItem(_EndItemID);
		break;
	}

	if (pEndItemObject == nullptr)
		return;

	// 서로의 위치에 넣어준다.
	switch (_EndslotId)
	{
	case Engine::UIID_SLOTBASIC:
		m_mapKeySlot[(INVENKEYSLOT)_EndUINumber] = pStartItemObject;
		m_bKeySlotEmpty[_EndUINumber] = true;
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		m_mapItemSlot[(INVENITEMSLOT)_EndUINumber] = pStartItemObject;
		m_bItemSlotEmpty[_EndUINumber] = true;
		break;

	case Engine::UIID_SLOTEMPTY:
		m_vecInventory.push_back(pStartItemObject);
		break;
	}

	switch (_StartslotId)
	{
	case Engine::UIID_SLOTBASIC:
		m_mapKeySlot[(INVENKEYSLOT)_StartUINumber] = pEndItemObject;
		m_bKeySlotEmpty[_StartUINumber] = true;
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		m_mapItemSlot[(INVENITEMSLOT)_StartUINumber] = pEndItemObject;
		m_bItemSlotEmpty[_StartUINumber] = true;
		break;

	case Engine::UIID_SLOTEMPTY:
		m_vecInventory.push_back(pEndItemObject);
		break;
	}
}

CGameObject* CInventory::Find_KeySlotItem(_uint _StartUINumber)
{
	CGameObject* pFindKeyItem = nullptr;

	switch (_StartUINumber) 
	{
	case 0:
		pFindKeyItem = m_mapKeySlot[KEYSLOT_ONE];
		m_mapKeySlot.erase(KEYSLOT_ONE);
		m_bKeySlotEmpty[KEYSLOT_ONE] = false;
		break;

	case 1:
		pFindKeyItem = m_mapKeySlot[KEYSLOT_TWO];
		m_mapKeySlot.erase(KEYSLOT_TWO);
		m_bKeySlotEmpty[KEYSLOT_TWO] = false;
		break;

	case 2:
		pFindKeyItem = m_mapKeySlot[KEYSLOT_THREE];
		m_mapKeySlot.erase(KEYSLOT_THREE);
		m_bKeySlotEmpty[KEYSLOT_THREE] = false;
		break;

	case 3:
		pFindKeyItem = m_mapKeySlot[KEYSLOT_FOUR];
		m_mapKeySlot.erase(KEYSLOT_FOUR);
		m_bKeySlotEmpty[KEYSLOT_FOUR] = false;
		break;

	case 4:
		pFindKeyItem = m_mapKeySlot[KEYSLOT_FIVE];
		m_mapKeySlot.erase(KEYSLOT_FIVE);
		m_bKeySlotEmpty[KEYSLOT_FIVE] = false;
		break;
	}

	return pFindKeyItem;
}

CGameObject* CInventory::Find_ItemSlotItem(_uint _StartUINumber)
{
	CGameObject* pFindItemSlotItem = nullptr;

	switch (_StartUINumber)
	{
	case 0:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_HELMET];
		m_mapItemSlot.erase(ITEMSLOT_HELMET);
		m_bItemSlotEmpty[ITEMSLOT_HELMET] = false;
		break;

	case 1:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_HAND];
		m_mapItemSlot.erase(ITEMSLOT_HAND);
		m_bItemSlotEmpty[ITEMSLOT_HAND] = false;
		break;

	case 2:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_ARMOR];
		m_mapItemSlot.erase(ITEMSLOT_ARMOR);
		m_bItemSlotEmpty[ITEMSLOT_ARMOR] = false;
		break;

	case 3:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_RING];
		m_mapItemSlot.erase(ITEMSLOT_RING);
		m_bItemSlotEmpty[ITEMSLOT_RING] = false;
		break;

	case 4:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_PANTS];
		m_mapItemSlot.erase(ITEMSLOT_PANTS);
		m_bItemSlotEmpty[ITEMSLOT_PANTS] = false;
		break;

	case 5:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_NECKLACE];
		m_mapItemSlot.erase(ITEMSLOT_NECKLACE);
		m_bItemSlotEmpty[ITEMSLOT_NECKLACE] = false;
		break;
	}

	return pFindItemSlotItem;
}

CGameObject* CInventory::Find_InvenSlotItem(ITEMTYPEID  _ItemID)
{
	for (auto&iter : m_vecInventory)
	{
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter)->Get_ItemTag();
			if (SlotItemType.eItemID == _ItemID.eItemID) {
				return iter;
			}
		}
	}

	return nullptr;
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

	for_each(m_vDead.begin(), m_vDead.end(), CDeleteObj());
	m_vDead.clear();

	CComponent::Free();
}