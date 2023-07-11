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
	void         Add_ItemObject(CGameObject* pGameObject); // 인벤토리에 아이템 추가
	void         delete_FindItem(ITEMID _itemId);          // 해당 아이디의 아이템 인벤토리에서 삭제

	CGameObject* Get_KeySlotObject(INVENKEYSLOT _key) { return m_mapKeySlot[_key]; }
	ITEMTYPEID   Get_KeySlotObjID(INVENKEYSLOT _key)  { return dynamic_cast<CItem*>(m_mapKeySlot[_key])->Get_ItemTag(); }

public:
	HRESULT			Ready_Inventory();
	virtual void	LateUpdate_Component() override;

private:
	map<INVENKEYSLOT, CGameObject*>  m_mapKeySlot;       // 키 슬롯 5칸
	map<INVENITEMSLOT, CGameObject*> m_mapItemSlot;      // 아이템 장착 슬롯 6칸
	vector<CGameObject*>             m_vecInventory;     // 내부 인벤토리 18칸

	_bool                            m_bKeySlotEmpty[KEYSLOT_END];  // 키 슬롯 아이템 보유 여부
	_bool                            m_bItemSlotEmpty[ITEMSLOT_END];// 아이템 슬롯 아이템 보유 여부

public:
	static	CInventory* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);

private:
	virtual void			Free();
};

END