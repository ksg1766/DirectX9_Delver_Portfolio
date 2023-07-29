#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Engine)

END

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

private:
	void	HekirekiIssen(const _float& fTimeDelta);

private:
	PD m_eCurr_PD = PD::Normal;
	PD m_ePrev_PD = PD::Normal;

	_bool	m_bReadyBreath = false;
	_float	m_fTimer = 0.f;

public:
	virtual void Free() override;
};

