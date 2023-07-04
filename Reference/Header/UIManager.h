#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class UI_Scene;
class UI_Popup;
class ENGINE_DLL CUIManager :
	public CBase
{
	DECLARE_SINGLETON(CUIManager)

public:
	explicit CUIManager();
	virtual ~CUIManager();

	void Update();
	void Render();

	void MakeWorldSpaceUI();
	void ShowSceneUI();
	void ShowPopupUI();
	void ClosePopupUI(UI_Popup* popup);
	void ClosePopupUI();

private:
	//stack<UI_Popup*> m_stackPopup;
	UI_Scene* m_pSceneUI;

	virtual void Free() override;
};

END