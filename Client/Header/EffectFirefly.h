#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectFirefly : public CTempEffect
{
private:
	explicit CEffectFirefly(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectFirefly();

public:
	void    Set_ChangeMode(_bool _bChange) { m_bChangeMode = _bChange; }

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT	Add_Component(void);

private:
	_float m_fLifeTime   = 0.f;
	_float m_fMoveTime   = 0.f;
	_float m_fChangeTime = 0.f;
	_float m_fMoveSpeed  = 0.f;
	_vec3  m_vecMoveDir;
	_bool  m_bChangeMode = false;

public:
	static CEffectFirefly*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

