#pragma once
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "Base.h"
#include "Export_Function.h"

BEGIN(Engine)

END

class CImGuiManager :
    public CBase
{
    DECLARE_SINGLETON(CImGuiManager)

private:
	explicit CImGuiManager();
	virtual ~CImGuiManager();

public:
	HRESULT	SetUp_ImGui();
	void	LateUpdate_ImGui();
	void	Render_ImGui(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void	Toggle_OnOff() { bUI_OnOff = !bUI_OnOff; }

private:
	_bool	bUI_OnOff;


public:
	virtual void		Free();

};

