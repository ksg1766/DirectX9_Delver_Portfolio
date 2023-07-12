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
	void	Key_Input(const _float& fTimeDelta);
	_vec3	Picking();

public:
	HRESULT	SetUp_ImGui();
	_int	Update_ImGui(const _float& fTimeDelta);
	void	LateUpdate_ImGui();
	void	Render_ImGui(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void	Toggle_OnOff() { bUI_OnOff = !bUI_OnOff; }
	HRESULT	OnSaveData();
	HRESULT	OnLoadData();

private:
	_bool	bUI_OnOff;

	vector<IDirect3DBaseTexture9*>		m_pTerainTexture;
	ImTextureID							selected_texture = nullptr;
	_uint								selected_texture_index = 0;


public:
	virtual void		Free();

};

