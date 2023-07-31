#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectWaterBubble : public CTempEffect
{
private:
	explicit CEffectWaterBubble(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectWaterBubble();
	
public:
	void    Set_OriginPos(_vec3 _pos) { m_vecOriginPos = _pos; }
	void    Set_BoundingBox(_vec3 _vecMin, _vec3 _vecMax) {
		m_EffectBoundingBox.vMin = _vecMin;
		m_EffectBoundingBox.vMax = _vecMax;
	}
	void    Set_EffectBubbleScale(_vec3 _vecMin, _vec3 _vecMax)
	{
		m_vecBubbleScale[0] = _vecMin;
		m_vecBubbleScale[1] = _vecMax;
	}

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT	Add_Component(void);

private:
	ParticleBoundingBox  m_EffectBoundingBox;
	_vec3  m_vecOriginPos;
	_vec3  m_vecBubbleScale[2];
	_float m_fSpeed;

public:
	static CEffectWaterBubble* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

