#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCubeBf;
class CTransform;
class CTexture;

END

class CSkyBox :	public Engine::CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox();

public:

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	CCubeBf*		m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;

public:
	static CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

