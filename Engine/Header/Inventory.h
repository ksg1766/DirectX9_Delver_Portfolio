#pragma once
#include "Component.h"
#include <Export_Utility.h>

BEGIN(Engine)

class ENGINE_DLL CInventory : public CComponent
{
private:
	explicit CInventory();
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory(const CInventory& rhs);
	virtual ~CInventory();

public:
	void Add_GameObjectInventory(CGameObject* pGameObject)
	{
		//UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT);

		// 키 슬롯이 비어있으면 해당 슬롯에 들어가고
		for (auto& iter : m_mapKeySlot) 
		{
			if (iter.second == nullptr) {
				iter.second = pGameObject;
				return;
			}
		}

		// 키 슬롯이 가득 차있을 시 내부 인벤에 넣는다.
		m_vecInventory.push_back(pGameObject);
	}

	CGameObject* Get_KeySlotObject(INVENKEYSLOT _key) {
		return m_mapKeySlot[_key];
	}
	CGameObject* Get_ItemSlotObject(INVENITEMSLOT _item) {
		return m_mapItemSlot[_item];
	}

public:
	HRESULT			Ready_Inventory();
	virtual void	LateUpdate_Component() override;

private:
	map<INVENKEYSLOT, CGameObject*>  m_mapKeySlot;
	map<INVENITEMSLOT, CGameObject*> m_mapItemSlot;
	vector<CGameObject*>             m_vecInventory;

public:
	static	CInventory* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);

private:
	virtual void			Free();
};

END