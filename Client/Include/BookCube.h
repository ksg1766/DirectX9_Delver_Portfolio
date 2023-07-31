#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
END

class CBookCube : public CGameObject
{
private:
	CBookCube(LPDIRECT3DDEVICE9 pGraphicDev);
	CBookCube(const CBookCube& rhs);
	virtual ~CBookCube();

public:
	virtual HRESULT	Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	HRESULT	Add_Component();

private:
	CCubeBf* m_pCubeBf = nullptr;
	CTexture* m_pTexture = nullptr;

public:
	static CBookCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

