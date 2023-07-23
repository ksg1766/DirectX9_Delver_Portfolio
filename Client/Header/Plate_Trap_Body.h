#pragma once
#include "Trap.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CCubeBf;
class CTexture;
class CStateMachine;
class CTempEffect;

END

class CPlate_Trap :
    public CTrap
{
private:
	explicit	CPlate_Trap(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CPlate_Trap(const CPlate_Trap& rhs);
	virtual		~CPlate_Trap();

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta)	override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	CCubeBf*		m_pCubeBf = nullptr;
	CTexture*		m_pTexture = nullptr;
	CGameObject*	m_pOtherObj = nullptr;

	_float			m_fInitialHeight;
	_float			m_fTime;
	_bool			m_bAttack;
private:
	HRESULT			Add_Component(void);
	void			Trap_On();
public:
	static CPlate_Trap* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

