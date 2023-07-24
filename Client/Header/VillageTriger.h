#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
END

class CVillageTriger : public CGameObject
{
private:
	explicit	CVillageTriger(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CVillageTriger(const CVillageTriger& rhs);
	virtual		~CVillageTriger();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	HRESULT		Add_Component(void);

private:
	CCubeBf*		m_pCubeBf = nullptr;
	CTexture*		m_pTexture = nullptr;

public:
	static CVillageTriger* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

