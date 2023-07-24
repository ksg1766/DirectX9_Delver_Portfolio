#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectBonfire : public CTempEffect
{
private:
	explicit CEffectBonfire(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectBonfire();

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);

private:
	_float      m_fMoveTime  = 0.f;
	_float      m_fMoveStopTime = 0.f;
	_float      m_fMoveSpeed = 0.f;

public:
	static CEffectBonfire* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};
