#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectTrace : public CTempEffect
{
private:
	explicit CEffectTrace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectTrace();

public:
	void    Set_TraceNumber(_float _fNumber) { m_fTraceNum = _fNumber; }

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT	Add_Component(void);

private:
	_float m_fTraceNum = 0.f;

public:
	static CEffectTrace* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

