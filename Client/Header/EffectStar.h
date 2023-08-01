#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectStar : public CTempEffect
{
private:
	explicit CEffectStar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectStar();

public:

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	_float      m_fScaleTime = 0.f;

public:
	static CEffectStar* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};