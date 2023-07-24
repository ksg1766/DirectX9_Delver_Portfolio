#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CFire : public CGameObject
{
private:
	explicit	CFire(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CFire(const CFire& rhs);
	virtual		~CFire();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	CRcTex*         m_pBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;

	_float          m_fFrame = 0.f;

private:
	HRESULT		Add_Component(void);

public:
	static CFire* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

