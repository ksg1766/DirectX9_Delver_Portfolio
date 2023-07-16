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

public:
	HRESULT	Ready_RigidBody();
	_int	Update_RigidBody(const _float& fTimeDelta);

	void	Set_Force(_vec3 _vForce) { m_vNetPower = _vForce; }
	void	Add_Force(_vec3 _vForce) { m_vNetPower += _vForce; }

	void	IsKinematic(_bool _bKinematic) { m_IsKinematic = _bKinematic; }
	void	UseGravity(_bool _bGravity) { m_UseGravity = _bGravity;	}

	_bool	IsKinematic() { return m_IsKinematic; }
	_bool	UseGravity() { return m_UseGravity; }

private:
	_bool	m_IsKinematic	= true;
	_bool	m_UseGravity	= true;
	
	_vec3	m_vNetPower;

	const _float	m_fGravityAcc = -9.8f;

public:
	static CRigidBody* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent * Clone(void) override;

private:
	virtual void	Free();
};

END