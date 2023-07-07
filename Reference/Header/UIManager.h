#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CTempUI;

class ENGINE_DLL CUIManager : public CBase
{
	DECLARE_SINGLETON(CUIManager)

private:
	explicit CUIManager();
	virtual ~CUIManager();

public:
	_bool Set_InvenUse(LPDIRECT3DDEVICE9& pGraphicDev)
	{
		HCURSOR Cursor = nullptr;

		if (m_bInven) {
			m_bInven = false;
			Show_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
		}
		else {
		    Cursor = GetCursor(); 
			Cursor = LoadCursor(NULL, IDC_ARROW);
			Hide_PopupUI(UIPOPUPLAYER::POPUP_MOUSE);
			Show_PopupUI(UIPOPUPLAYER::POPUP_INVEN);
			m_bInven = true;
		}

		SetCursor(Cursor);
		return m_bInven;
	}

	_bool Set_StatUse()
	{
		HCURSOR Cursor = nullptr;

		if (m_bStat) {
			m_bStat = false;
		}
		else {
			Cursor = GetCursor();
			Cursor = LoadCursor(NULL, IDC_ARROW);
			m_bStat = true;
		}

		SetCursor(Cursor);
		return m_bStat;
	}

	_bool Set_MapUse()
	{
		HCURSOR Cursor = nullptr;

		if (m_bMap) {
			m_bMap = false;
		}
		else {
			Cursor = GetCursor();
			Cursor = LoadCursor(NULL, IDC_ARROW);
			m_bMap = true;
		}

		SetCursor(Cursor);
		return m_bMap;
	}

	_bool Set_EscUse()
	{
		HCURSOR Cursor = nullptr;

		if (m_bEsc) {
			m_bEsc = false;
		}
		else
		{
			Cursor = GetCursor();
			Cursor = LoadCursor(NULL, IDC_ARROW);
			m_bEsc = true;
		}
		
		SetCursor(Cursor);
		return m_bEsc;
	}

	void Show_PopupUI(UIPOPUPLAYER _PopupID);
	void Hide_PopupUI(UIPOPUPLAYER _PopupID);

public:
	// 정보 전달 get/ set 등 함수 추가
	void AddBasicGameobject_UI(UILAYER eType, CGameObject* pGameObject);
	void AddPopupGameobject_UI(UIPOPUPLAYER ePopupLayer, UILAYER eType, CGameObject* pGameObject);

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

private:
	//POINT m_MousePoint = { 0.f, 0.f };
	_bool m_bInven = false;
	_bool m_bStat = false;
	_bool m_bMap = false;
	_bool m_bEsc = false;

	vector<CGameObject*> m_vecUIbasic[UILAYER::UI_END];
	map<UIPOPUPLAYER, vector<CGameObject*>[UILAYER::UI_END]> m_mapPpopupUI;

private:
	virtual void Free() override;
};

END