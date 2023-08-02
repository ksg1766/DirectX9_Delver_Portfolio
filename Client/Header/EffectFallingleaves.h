#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectFallingleaves : public CTempEffect
{
private:
	explicit CEffectFallingleaves(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectFallingleaves();

public:
	void    Set_ChangeMode(_bool _bChange) { m_bChange = _bChange; }

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	vector<CGameObject*> m_vecLeaf;
	_float m_fMoveTime   = 0.f;
	_float m_fChangeTime = 0.f;
	_bool  m_bChange = false;

public:
	static CEffectFallingleaves*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

