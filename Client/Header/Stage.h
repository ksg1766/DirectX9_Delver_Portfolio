#pragma once

#include "Scene.h"
#include "Terrain.h"
#include "DynamicCamera.h"
#include "SkyBox.h"
#include "Player.h"
#include "CubeBlock.h"
#include "UIplayerhp.h"
#include "UIemptyslot.h"
#include "UItooltip.h"
#include "UIplayerstat.h"
#include "UIaimpoint.h"
#include "SkeletonKing.h"
#include "BossProjectile.h"
#include "BossExplosion.h"

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

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

