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

		if (m_bInven) { // 인벤 닫을 때 
			m_bInven = false;
			Show_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			CGameObject* Obj = Get_Object(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 0);
			dynamic_cast<CTempUI*>(Obj)->Set_UIImage(1);
		}
		else { // 인벤 열 때
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
			Show_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			CGameObject* Obj = Get_Object(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 0);
			dynamic_cast<CTempUI*>(Obj)->Set_UIImage(0);
			CGameObject* Obj2 = Get_Object(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 1);
			dynamic_cast<CTempUI*>(Obj2)->Set_UIImage(3);
		}

		SetCursor(Cursor);
		return m_bInven;
	}
	_bool Set_StatUse()
	{
		HCURSOR Cursor = nullptr;

		if (m_bStat) { // 스탯창 닫을 때
			m_bStat = false;
			Show_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_STAT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			CGameObject* Obj = Get_Object(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 1);
			dynamic_cast<CTempUI*>(Obj)->Set_UIImage(3);
		}
		else { // 스탯창 열 때
			m_bInven = false;
			m_bMap = false;
			m_bEsc = false;

			m_bStat = true;
			Cursor = GetCursor();
			Cursor = LoadCursor(NULL, IDC_ARROW);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MAP);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_ESC);

			Show_PopupUI(UIPOPUPLAYER::POPUP_STAT);
			Show_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			CGameObject* Obj = Get_Object(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 0);
			dynamic_cast<CTempUI*>(Obj)->Set_UIImage(1);
			CGameObject* Obj2 = Get_Object(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIOBJECTTTAG::UIID_INVENBUTTON, 1);
			dynamic_cast<CTempUI*>(Obj2)->Set_UIImage(2);
		}

		SetCursor(Cursor);
		return m_bStat;
	}

	_bool Set_MapUse()
	{
		HCURSOR Cursor = nullptr;

		if (m_bMap) { // 지도 닫을 때
			m_bMap = false;
			Show_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MAP);
		}
		else { // 지도 열 때
			m_bInven = false;
			m_bStat = false;
			m_bEsc = false;

			m_bMap = true;
			Cursor = GetCursor();
			Cursor = LoadCursor(NULL, IDC_ARROW);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
            Hide_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
            Hide_PopupUI(UIPOPUPLAYER::POPUP_STAT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_ESC);

			Show_PopupUI(UIPOPUPLAYER::POPUP_MAP);
		}

		SetCursor(Cursor);
		return m_bMap;
	}
	_bool Set_EscUse()
	{
		HCURSOR Cursor = nullptr;

		if (m_bEsc) { // Esc 닫을 때
			m_bEsc = false;
			Show_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_ESC);
		}
		else // Esc 열 때 
		{
			m_bInven = false;
			m_bStat = false;
			m_bMap = false;

			m_bEsc = true;
			Cursor = GetCursor();
			Cursor = LoadCursor(NULL, IDC_ARROW);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_EQUIPMENT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_STAT);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MAP);

			Show_PopupUI(UIPOPUPLAYER::POPUP_ESC);
		}
		
		SetCursor(Cursor);
		return m_bEsc;
	}

	void Show_PopupUI(UIPOPUPLAYER _PopupID);
	void Hide_PopupUI(UIPOPUPLAYER _PopupID);

	void Delete_BasicObject(UILAYER eType);

public:
	void AddBasicGameobject_UI(UILAYER eType, CGameObject* pGameObject);
	void AddPopupGameobject_UI(UIPOPUPLAYER ePopupLayer, UILAYER eType, CGameObject* pGameObject);
	void AddItemGameobject_UI(CGameObject* pGameObject);
	CGameObject* Get_Object(UIPOPUPLAYER ePopupLayer, UILAYER eType, UIOBJECTTTAG eObjID, _uint eUINumber)
	{ 
		UIOBJECTTTAG UIObjID;
		_uint        UINumber;

		for (auto iter : m_mapPpopupUI[ePopupLayer][eType])
		{
			dynamic_cast<CTempUI*>(iter)->Get_UIObjID(UIObjID, UINumber);
			if (UIObjID == eObjID && UINumber == eUINumber)
			{
				return iter;
			}
		}
	}
	CGameObject* Get_BasicObject(UILAYER eType, UIOBJECTTTAG eObjID, _uint eUINumber)
	{
		UIOBJECTTTAG UIObjID;
		_uint        UINumber;

		for (auto iter : m_vecUIbasic[eType])
		{
			dynamic_cast<CTempUI*>(iter)->Get_UIObjID(UIObjID, UINumber);
			if (UIObjID == eObjID && UINumber == eUINumber)
			{
				return iter;
			}
		}
	}

public:
	_int Update_UI(const _float& fTimeDelta);
	void LateUpdate_UI();
	void Render_UI(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	//void MakeWorldSpaceUI();
	//void ShowSceneUI();
	//void ShowPopupUI();
	//void ClosePopupUI(UI_Popup* popup);
	//void ClosePopupUI();

public:
	_bool m_bInven = false;
	_bool m_bStat  = false;
	_bool m_bMap   = false;
	_bool m_bEsc   = false;

private:
	vector<CGameObject*> m_vecUIbasic[UILAYER::UI_END];
	map<UIPOPUPLAYER, vector<CGameObject*>[UILAYER::UI_END]> m_mapPpopupUI;

	vector<CGameObject*> m_vecCreate;
	vector<CGameObject*> m_vecDead;

private:
	virtual void Free() override;
};

END