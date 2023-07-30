#pragma once

#include "Scene.h"
#include "DynamicCamera.h"
#include "SkyBox.h"
#include "Player.h"
#include "FootRay.h"
#include "CubeBlock.h"
#include "Water.h"
#include "WaterFall.h"
#include "DynamicCubeBlock.h"

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
#include "Npc_Bard.h"

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
	void	Set_Bgm(_bool _bgm)		 { m_bBgmStop = _bgm; }
	void	Set_Sound(_float _Sound) { m_fSound = _Sound; }
	void	Set_Sound(_bool _Sound)	 { m_bSoundOn = _Sound; }

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_bool	m_bBgmStop = true;
	_bool	m_bSoundOn = true;

	_float	m_fSound;
	const _float m_fMaxVolume = 1.f;
	const _float m_fMinVolume = 0.f;
	const _float m_fMaxDistance = 120.f;

private:
	virtual void Free() override;

};

