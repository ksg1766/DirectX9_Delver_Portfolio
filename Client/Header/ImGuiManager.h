#pragma once
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "Base.h"
#include "Export_Function.h"

BEGIN(Engine)

END

class CSpawningPool;
class CImGuiManager :
    public CBase
{
    DECLARE_SINGLETON(CImGuiManager)

private:
	explicit CImGuiManager();
	virtual ~CImGuiManager();

public:
	void	Key_Input(const _float& fTimeDelta);
	_vec3	PickingBlock();
	_vec3	PickingSpawner();
	_vec3	PickingTrap();
	//_vec3	PickingEnvironmental();

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

	enum TOOLMODE { MAP, SPAWNER, TRAP, TOOL_END };
	TOOLMODE							m_eToolMode = TOOL_END;

	// Map Tool
	vector<IDirect3DBaseTexture9*>		m_pCubeTexture;

	ImTextureID							selected_texture = nullptr;
	_uint								selected_texture_index = 0;

	_uint								m_iPickingMode;

	// Spawner Tool
	_float								m_fSpawnTime;
	_int								m_iSpawnCapacity;
	_float								m_fSpawnRadius;
	MONSTERTAG							m_eSpawnerTag;

	_bool								m_bDisposable;

	// Monster Tool

	// Trap Tool
	_uint								m_iSelected_index = 0;

	// Environmanet Tool

public:
	virtual void		Free();

};

