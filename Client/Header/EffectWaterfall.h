#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectWaterfall : public CTempEffect
{
private:
	explicit CEffectWaterfall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectWaterfall();

public:
	void    Set_BoundingBox(_vec3 _vecMin, _vec3 _vecMax) { 
		m_EffectBoundingBox.vMin = _vecMin;
		m_EffectBoundingBox.vMax = _vecMax;
	}

	void    Set_EffectRotation() {}

	void    Set_EffectDropHeight(_float _height) { m_fHeight = _height; }
	void    Set_EffectDropScale(_uint _Count, _vec3 _vecMin, _vec3 _vecMax)   
	{ 
		m_iDropCount = _Count;
		m_vecDropScale[0] = _vecMin;
		m_vecDropScale[1] = _vecMax;
	}
	void    Set_EffectMoveScale(_uint _Count, _vec3 _vecMin, _vec3 _vecMax)
	{
		m_iMoveCount = _Count;
		m_vecMoveScale[0] = _vecMin;
		m_vecMoveScale[1] = _vecMax;
	}
	void    Set_EffectBubbleScale(_uint _Count, _vec3 _vecMin, _vec3 _vecMax)
	{
		m_iBubbleCount = _Count;
		m_vecBubbleScale[0] = _vecMin;
		m_vecBubbleScale[1] = _vecMax;
	}
	void    Set_EffectMoveSet(_uint _Set) { m_iMoveSet = _Set; }
	// 0 버블만 생성 / 1 기본 // 2 90도 회전

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT	Add_Component(void);

private:
	ParticleBoundingBox  m_EffectBoundingBox;

	vector<CGameObject*> m_vecDrop;
	vector<CGameObject*> m_vecMove;
	vector<CGameObject*> m_vecBubble;

	_uint m_iDropCount   = 0;
	_uint m_iMoveCount   = 0;
	_uint m_iBubbleCount = 0;

	_vec3 m_vecDropScale[2];
	_vec3 m_vecMoveScale[2];
	_vec3 m_vecBubbleScale[2];

	_float m_fMoveTime;
	_float m_fChangeTime;
	_int   m_iParticleNumber;

	_uint  m_iMoveSet;
	_float m_fHeight;

public:
	static CEffectWaterfall* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

