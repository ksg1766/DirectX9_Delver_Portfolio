#pragma once
#include "Cube.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class COrbBlockLight : public Engine::CGameObject
{
protected:
	explicit COrbBlockLight(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit COrbBlockLight(const COrbBlockLight& rhs);
	virtual ~COrbBlockLight();

public:

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CTexture*   m_pTexture = nullptr;
	CRcTex*	    m_pBuffer = nullptr;

public:
	static COrbBlockLight* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;
};
