#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
class CStateMachine;
END
class CProjectile_Trap_Blade;

class CProjectile_Trap :
    public CGameObject
{
private:
	explicit	CProjectile_Trap(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CProjectile_Trap(const CProjectile_Trap& rhs);
	virtual		~CProjectile_Trap();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
	
	void		Create_Projectile();
private:
	CCubeBf*		m_pCubeBf = nullptr;
	CTexture*		m_pTexture = nullptr;

private:
	HRESULT		Add_Component(void);

public:
	static CProjectile_Trap* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

