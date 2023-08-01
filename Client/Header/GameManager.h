#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Engine)

END

class CPlayer;
class CFlyingCamera;
class CSkeletonKing;
class CGameManager :
    public CBase
{
	DECLARE_SINGLETON(CGameManager)

private:
	explicit CGameManager();
	virtual ~CGameManager();

public:
	_int	Update_Game(const _float& fTimeDelta);
	void	LateUpdate_Game();
	void	Render_Game(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void	PlayMode(PD _ePD_Mode) {  m_ePrev_PD = m_eCurr_PD; m_eCurr_PD = _ePD_Mode; }

public:
	void	SetTimer(_float& _fTimer) { m_fTimer = _fTimer; }

private:
	void	ShowVillage(const _float& fTimeDelta);
	void	ShowSewer(const _float& fTimeDelta);
	void	ShowBoss(const _float& fTimeDelta);
	void	ShowMiniBoss(const _float& fTimeDelta);
	void	HekirekiIssen(const _float& fTimeDelta);

private:
	CPlayer*		m_pPlayer;
	CFlyingCamera*	m_pCamera;
	CSkeletonKing*	m_pBoss;

	PD m_eCurr_PD = PD::Normal;
	PD m_ePrev_PD = PD::Normal;

	_float	m_fTimer = 10.f;
	
	_uint	m_iVisitCount = 0;

	// HekiRekiIssen
	_bool	m_bReadyBreath = false;
	_bool	m_bEffectCreated[3] = {0};

public:
	virtual void Free() override;
};

