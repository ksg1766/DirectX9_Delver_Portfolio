#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectWaterMove : public CTempEffect
{
private:
	explicit CEffectWaterMove(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectWaterMove();

public:
	void    Set_OriginPos(_vec3 _pos) { m_vecOriginPos = _pos; }
	void    Set_BoundingBox(_vec3 _vecMin, _vec3 _vecMax) {
		m_EffectBoundingBox.vMin = _vecMin;
		m_EffectBoundingBox.vMax = _vecMax;
	}
	void    Set_EffectMoveScale(_vec3 _vecMin, _vec3 _vecMax)
	{
		m_vecMoveScale[0] = _vecMin;
		m_vecMoveScale[1] = _vecMax;
	}
	void    Set_PickNumber(_uint iPickNumber, _uint iMaxNumber) 
	{
		m_PickNumber = iPickNumber; 
		m_MaxNumber  = iMaxNumber;
	}
	void    Set_EffectMoveSet(_uint _Set) { m_iMoveSet = _Set; }
	// 큰 순 0 / 작은 순 1

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
	_vec3  m_vecMoveScale[2];
	_uint  m_PickNumber = 0;
	_uint  m_MaxNumber = 0;
	_float m_fSpeed;
	_bool  m_bRight = false;
	_bool  m_bLeft  = false;

	_uint  m_iMoveSet;

public:
	static CEffectWaterMove* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

