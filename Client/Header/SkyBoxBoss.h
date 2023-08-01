#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCubeBf;
class CTransform;
class CTexture;

END

class CSkyBoxBoss :	public Engine::CGameObject
{
private:
	explicit CSkyBoxBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkyBoxBoss(const CSkyBoxBoss& rhs);
	virtual ~CSkyBoxBoss();

public:

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	CCubeBf*		m_pCubeBf = nullptr;
	CTexture*		m_pTexture = nullptr;

public:
	static CSkyBoxBoss*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

