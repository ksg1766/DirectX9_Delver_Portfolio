#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRigidBody :
	public CComponent
{
private:
	explicit CRigidBody();
	explicit CRigidBody(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRigidBody(const CRigidBody& rhs);
	virtual ~CRigidBody();

	// CComponent을(를) 통해 상속됨
	virtual CComponent * Clone(void) override;

private:
	virtual void			Free();
};

END