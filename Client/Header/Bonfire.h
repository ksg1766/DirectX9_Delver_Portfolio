#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
END

class CBonfire : public CGameObject
{
private:
	explicit	CBonfire(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBonfire(const CBonfire& rhs);
	virtual		~CBonfire();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	CCubeBf*		m_pCubeBf = nullptr;
	CTexture*		m_pTexture = nullptr;
	_float			m_fTime;

private:
	HRESULT		Add_Component(void);

public:
	static CBonfire* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

