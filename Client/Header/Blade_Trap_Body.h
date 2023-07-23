#pragma once
#include "Trap.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
class CStateMachine;
END
class CBlade_Trap_Blade;

class CBlade_Trap :
    public CTrap
{
private:
	explicit	CBlade_Trap(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBlade_Trap(const CBlade_Trap& rhs);
	virtual		~CBlade_Trap();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
	
	void		Create_Blade();
private:
	CCubeBf*		m_pCubeBf = nullptr;
	CTexture*		m_pTexture = nullptr;

	CBlade_Trap_Blade* m_pTrapBlade;

	_bool			m_bSpawnBlade;
	_vec3			m_vBladePos[9];
	_vec3			m_vBladeDir[7];
private:
	HRESULT		Add_Component(void);

public:
	static CBlade_Trap* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

