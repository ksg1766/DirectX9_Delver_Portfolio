#pragma once

#include "Scene.h"
#include "Terrain.h"
#include "DynamicCamera.h"
#include "SkyBox.h"
#include "Player.h"
#include "FootRay.h"
#include "CubeBlock.h"

#include "UIplayerhp.h"
#include "UIplayerstat.h"
#include "UIaimpoint.h"
#include "UIbutton.h"
#include "UIemptyslot.h"
#include "UIbasicslot.h"
#include "UIequipmentslot.h"
#include "UIbigmap.h"
#include "UIEscBackground.h"
#include "UIResumeButton.h"
#include "UIOptionsButton.h"
#include "UIQuitButton.h"
#include "UISpeech_OldMan.h"//Speech Bubble Test

#include "SkeletonKing.h"
#include "Npc_OldMan.h"

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();

public:
	virtual HRESULT Ready_Scene()							override;
	virtual _int	Update_Scene(const _float& fTimeDelta)	override;
	virtual void	LateUpdate_Scene()						override;
	virtual void	Render_Scene()							override;

private:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_Layer_Environment(LAYERTAG _eLayerTag);
	HRESULT			Ready_Layer_GameLogic(LAYERTAG _eLayerTag);
	HRESULT			Ready_Layer_UI(LAYERTAG _eLayerTag);
	virtual HRESULT	Load_Data() override;

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

