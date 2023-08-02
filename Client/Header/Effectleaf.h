#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectleaf : public CTempEffect
{
private:
	explicit CEffectleaf(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectleaf();

public:
	void    Set_ChangeMode(_bool _bChange) { m_bChangeMode = _bChange; }
	void    Change_Move(_vec3 _NewDir, _float _Speed)
	{ 
		m_vecMoveDir = _NewDir;
		m_bMoveSpeed = _Speed;
	}

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT	Add_Component(void);

private:
	_float m_bMoveSpeed = 0.f;
	_float m_fCount = 0.f;
	_bool  m_bFall = false;
	_vec3  m_vecMoveDir;
	_bool  m_bChangeMode = false;
	_bool  m_bChangeImage = false;

public:
	static CEffectleaf*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

