#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectWaterMove : public CTempEffect
{
private:
	explicit CEffectWaterMove(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectWaterMove();

public:
	void    Set_EffectMoveScale(_vec3 _vecMin, _vec3 _vecMax)
	{
		m_vecMoveScale[0] = _vecMin;
		m_vecMoveScale[1] = _vecMax;
	}

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT	Add_Component(void);

private:
	_vec3  m_vecMoveScale[2];

public:
	static CEffectWaterMove* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

