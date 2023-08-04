#pragma once
#include "Engine_Define.h"
#include "Base.h"
#include "TempUI.h"

#include "Export_Utility.h"

BEGIN(Engine)

//class CTempUI;

class ENGINE_DLL CUIManager : public CBase
{
	DECLARE_SINGLETON(CUIManager)

private:
	explicit CUIManager();
	virtual ~CUIManager();

public:
	_bool Set_InvenUse()
	{
		HCURSOR Cursor = nullptr;

		if (m_bInven) { // ¿Œ∫• ¥›¿ª ∂ß 
			m_bInven = false;
			Show_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
			Hide_InvenItem(0);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			CGameObject* Obj = nullptr;
			Obj = Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 0);
			static_cast<CTempUI*>(Obj)->Set_UIImage(1);
		}
		else { // ¿Œ∫• ø≠ ∂ß
			m_bStat = false;
			m_bMap = false;
			m_bEsc = false;

			m_bInven = true;
		    Cursor = GetCursor(); 
			Cursor = LoadCursor(NULL, IDC_ARROW);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_STAT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MAP);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_ESC);

			Show_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
			Show_InvenItem(0);
			Show_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			CGameObject* Obj = nullptr;
			Obj = Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 0);
			static_cast<CTempUI*>(Obj)->Set_UIImage(0);
			CGameObject* Obj2 = nullptr;
			Obj2 = Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 1);
			static_cast<CTempUI*>(Obj2)->Set_UIImage(3);
		}

		SetCursor(Cursor);
		return m_bInven;
	}
	_bool Set_StatUse()
	{
		HCURSOR Cursor = nullptr;

		if (m_bStat) { // Ω∫≈»√¢ ¥›¿ª ∂ß
			m_bStat = false;
			Show_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_STAT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			Hide_InvenItem(0);
			CGameObject* Obj = nullptr;
			Obj = Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 1);
			static_cast<CTempUI*>(Obj)->Set_UIImage(3);
		}
		else { // Ω∫≈»√¢ ø≠ ∂ß
			m_bInven = false;
			m_bMap = false;
			m_bEsc = false;

			m_bStat = true;
			Cursor = GetCursor();
			Cursor = LoadCursor(NULL, IDC_ARROW);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
			Hide_InvenItem(1);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MAP);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_ESC);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_SHOP);

			Show_PopupUI(UIPOPUPLAYER::POPUP_STAT);
			Show_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			CGameObject* Obj = nullptr;
			Obj = Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 0);
			static_cast<CTempUI*>(Obj)->Set_UIImage(1);
			CGameObject* Obj2 = nullptr;
			Obj2 = Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 1);
			static_cast<CTempUI*>(Obj2)->Set_UIImage(2);
		}

		SetCursor(Cursor);
		return m_bStat;
	}

	_bool Set_MapUse()
	{
		HCURSOR Cursor = nullptr;

		if (m_bMap) { // ¡ˆµµ ¥›¿ª ∂ß
			m_bMap = false;
			Show_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MAP);
		}
		else { // ¡ˆµµ ø≠ ∂ß
			m_bInven = false;
			m_bStat = false;
			m_bEsc = false;

			m_bMap = true;
			Cursor = GetCursor();
			Cursor = LoadCursor(NULL, IDC_ARROW);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
			Hide_InvenItem(0);
            Hide_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
            Hide_PopupUI(UIPOPUPLAYER::POPUP_STAT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_ESC);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_SHOP);

			Show_PopupUI(UIPOPUPLAYER::POPUP_MAP);
		}

		SetCursor(Cursor);
		return m_bMap;
	}
	_bool Set_EscUse()
	{
		HCURSOR Cursor = nullptr;

		if (m_bEsc) { // Esc ¥›¿ª ∂ß
			m_bEsc = false;
			Show_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_ESC);
		}
		else // Esc ø≠ ∂ß 
		{
			m_bInven = false;
			m_bStat = false;
			m_bMap = false;
			m_bSpeech = false;
			m_bEsc = true;
			Cursor = GetCursor();
			Cursor = LoadCursor(NULL, IDC_ARROW);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
			Hide_InvenItem(0);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_STAT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MAP);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_SHOP);
			Show_PopupUI(UIPOPUPLAYER::POPUP_ESC);
		}
		
		SetCursor(Cursor);
		return m_bEsc;
	}

	_bool Set_Shop()
	{
		HCURSOR Cursor = nullptr;

		if (m_bShop)
		{
			m_bShop = false;
			Show_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_SHOP);
		}
		else 
		{
			m_bInven = false;
			m_bStat = false;
			m_bMap = false;
			m_bSpeech = false;
			m_bEsc = false;
			m_bShop = true;
			Cursor = GetCursor();
			Cursor = LoadCursor(NULL, IDC_ARROW);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
			Hide_InvenItem(0);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_STAT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MAP);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_ESC);
			Show_PopupUI(UIPOPUPLAYER::POPUP_SHOP);
		}

		SetCursor(Cursor);
		return m_bShop;
	}

	_bool Set_SpeechBubbleUse()//Speech Bubble Test
	{
		HCURSOR Cursor = nullptr;

		if (m_bSpeech) { // ∏ª«≥º± ¥›¿ª ∂ß
			m_bSpeech = false;
			Show_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
		}
		else { // ∏ª«≥º± ø≠ ∂ß
			m_bInven = false;
			m_bStat = false;
			m_bEsc = false;
			m_bMap = false;
			m_bSpeech = true;
			Cursor = GetCursor();
			Cursor = LoadCursor(NULL, IDC_ARROW);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
			Hide_InvenItem(0);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_STAT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MAP);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_ESC);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_SHOP);

			Show_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
		}

		SetCursor(Cursor);
		return m_bSpeech;
	}

	void  Show_PopupUI(UIPOPUPLAYER _PopupID);
	void  Hide_PopupUI(UIPOPUPLAYER _PopupID);

	void  Delete_BasicObject(UILAYER eType);
	void  Delete_FindItemUI(ITEMTYPEID _itemId);

	void  Show_InvenItem(_uint iType);
	void  Hide_InvenItem(_uint iType);

public:
	void  Add_BasicGameobject(UILAYER eType, CGameObject* pGameObject);
	void  Add_PopupGameobject(UIPOPUPLAYER ePopupLayer, UILAYER eType, CGameObject* pGameObject);
	void  Add_ItemGameobject(CGameObject* pGameObject);
	void  ReplayAdd_ItemGameobject(CGameObject* pGameObject);

	CGameObject* Get_ItemUI(ITEMID _eItemID);
	CGameObject* Get_PopupObject(UIPOPUPLAYER ePopupLayer, UILAYER eType, UIOBJECTTTAG eObjID, _uint eUINumber)
	{ 
		UIOBJECTTTAG UIObjID;
		_uint        UINumber;

		for (auto iter : m_mapPpopupUI[ePopupLayer][eType])
		{
			static_cast<CTempUI*>(iter)->Get_UIObjID(UIObjID, UINumber);
			if (UIObjID == eObjID && UINumber == eUINumber)
			{
				return iter;
			}
		}
	}
	CGameObject* Get_PopupObjectBasicSlot(ITEMTYPEID ItemType);
	CGameObject* Get_BasicObject(UILAYER eType, UIOBJECTTTAG eObjID, _uint eUINumber)
	{
		UIOBJECTTTAG UIObjID;
		_uint        UINumber;

		for (auto iter : m_vecUIbasic[eType])
		{
			static_cast<CTempUI*>(iter)->Get_UIObjID(UIObjID, UINumber);
			if (UIObjID == eObjID && UINumber == eUINumber)
			{
				return iter;
			}
		}
	}

	void         Set_ColliderSlot(UIOBJECTTTAG _SlotTag, _uint _UINumber, _bool _Setbool);
	CGameObject* Find_ColliderSlot();
	void         Set_PickingItemUI(CGameObject* _pObject) { m_pPickingObject = _pObject; }
	CGameObject* Get_PickingItemUI() { return m_pPickingObject; }

public:
	_int  Update_UI(const _float& fTimeDelta);
	void  LateUpdate_UI();
	void  Render_UI(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	//void MakeWorldSpaceUI();
	//void ShowSceneUI();
	//void ShowPopupUI();
	//void ClosePopupUI(UI_Popup* popup);
	//void ClosePopupUI();
public:
	_bool m_bInven    = false;
	_bool m_bStat     = false;
	_bool m_bMap      = false;
	_bool m_bEsc	  = false;
	_bool m_bSpeech   = false;
	_bool m_bShop	  = false;
	_bool m_bMouse    = false;

	_bool m_bSlotBasicCollider[5];     // UIOBJECTTTAG::UIID_SLOTBASIC
	_bool m_bSlotEquipmentCollider[6]; // UIOBJECTTTAG::UIID_SLOTEQUIPMENT
	_bool UIID_SLOTEMPTYCollider[18];  // UIOBJECTTTAG::UIID_SLOTEMPTY

private:
	vector<CGameObject*> m_vecUIbasic[UILAYER::UI_END];
	map<UIPOPUPLAYER, list<CGameObject*>[UILAYER::UI_END]> m_mapPpopupUI;

	//vector<CGameObject*> m_vecCreate;
	vector<CGameObject*> m_vecItemDead;
	vector<CGameObject*> m_vecDead;

	CGameObject* m_pPickingObject = nullptr;

	//vector<CGameObject*> m_vecBasicItem;

private:
	virtual void Free() override;
};

END