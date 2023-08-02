#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

BEGIN(Engine)

class CRigidBody;

END

class CEffectBlood : public CTempEffect
{
private:
	explicit CEffectBlood(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectBlood();

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT	Add_Component(void);

private:
	CRigidBody* m_pRigidBody = nullptr;

public:
	static CEffectBlood*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

