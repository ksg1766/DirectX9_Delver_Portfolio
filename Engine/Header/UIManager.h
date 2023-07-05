#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

//class CTempUI;

class ENGINE_DLL CUIManager : public CBase
{
	DECLARE_SINGLETON(CUIManager)

private:
	explicit CUIManager();
	virtual ~CUIManager();

public:
	// 정보 전달 get/ set 등 함수 추가
	void Add_UIGameobject(UILAYER eType, CGameObject* pGameObject);

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
	vector<CGameObject*> m_UIList[UILAYER::UI_END];
	//stack<UI_Popup*> m_stackPopup;

private:
	virtual void Free() override;
};

END