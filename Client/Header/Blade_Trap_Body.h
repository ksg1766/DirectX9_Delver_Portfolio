#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
class CStateMachine;
END

class CBlade_Trap :
    public CGameObject
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

private:
	CCubeBf*		m_pCubeBf = nullptr;
	CTexture*		m_pTexture[(_uint)STATE::STATE_END] = {};
	CStateMachine*	m_pStateMachine = nullptr;

private:
	HRESULT		Add_Component(void);

public:
	static CBlade_Trap* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

