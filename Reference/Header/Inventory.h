#pragma once
#include "Component.h"

#include "Item.h" // 아이템 부모 클래스

BEGIN(Engine)

class ENGINE_DLL CInventory : public CComponent
{
private:
	explicit CInventory();
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory(const CInventory& rhs);
	virtual ~CInventory();

public:
	void            Add_ItemObject(CGameObject* pGameObject); // 인벤토리에 아이템 추가
	void            delete_FindItem(ITEMTYPEID _itemId);      // 해당 아이디의 아이템 인벤토리에서 삭제 또는 카운트 감소

	CGameObject*    Get_KeySlotObject(INVENKEYSLOT _key) { return m_mapKeySlot[_key]; }
	ITEMTYPEID      Get_KeySlotObjID(INVENKEYSLOT _key)  { return dynamic_cast<CItem*>(m_mapKeySlot[_key])->Get_ItemTag(); }

	void            Switch_InvenItem(ITEMTYPEID _itemId, UIOBJECTTTAG _slotId, _uint _UINumber); // 아이템 아이디와 옮길려는 슬롯 위치 아이디

public:
	HRESULT			Ready_Inventory();
	virtual void	LateUpdate_Component() override;

private:
	void            Switch_Keyslot(INVENKEYSLOT _key, ITEMTYPEID _itemId, UIOBJECTTTAG _slotId, _uint _UINumber);
	void            Switch_Itemslot(INVENITEMSLOT _key, ITEMTYPEID _itemId, UIOBJECTTTAG _slotId, _uint _UINumber);

private:
	map<INVENKEYSLOT, CGameObject*>  m_mapKeySlot;       // 키 슬롯 5칸
	map<INVENITEMSLOT, CGameObject*> m_mapItemSlot;      // 아이템 장착 슬롯 6칸
	vector<CGameObject*>             m_vecInventory;     // 내부 인벤토리 18칸

	_bool                            m_bKeySlotEmpty[KEYSLOT_END];  // 키 슬롯 아이템 보유 여부
	_bool                            m_bItemSlotEmpty[ITEMSLOT_END];// 아이템 슬롯 아이템 보유 여부

	_bool                            m_bSwitch;

public:
	static	CInventory* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);

private:
	virtual void			Free();
};

END