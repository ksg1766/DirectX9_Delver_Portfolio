#pragma once

#include "GameObject.h"

BEGIN(Engine)

END

class CSpiderRay : public CGameObject
{
private:
	explicit CSpiderRay(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSpiderRay(const CSpiderRay& rhs);
	virtual ~CSpiderRay();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
	void	Set_Host(CGameObject* _host) { m_pHost = _host; }

public:
	CGameObject* m_pHost;
	CGameObject* m_pColTarget = nullptr;

private:
	HRESULT		Add_Component();

public:
	static CSpiderRay* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

