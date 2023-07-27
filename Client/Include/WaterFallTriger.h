#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
END

class CWaterFallTriger : public CGameObject
{
private:
	CWaterFallTriger(LPDIRECT3DDEVICE9 pGraphicDev);
	CWaterFallTriger(const CWaterFallTriger& rhs);
	virtual ~CWaterFallTriger();

public:
	virtual HRESULT	Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;

private:
	HRESULT	Add_Component();

private:
	CCubeBf*	m_pCubeBf = nullptr;
	CTexture* m_pTexture = nullptr;

	_float	m_fSound;
	const _float m_fMaxVolume = 1.f;
	const _float m_fMinVolume = 0.f;
	const _float m_fMaxDistance = 120.f;

public:
	static CWaterFallTriger* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

