#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
class CStateMachine;
class CTempEffect;
END

class CJump_Plate :
    public CGameObject
{
private:
	explicit	CJump_Plate(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CJump_Plate(const CJump_Plate& rhs);
	virtual		~CJump_Plate();

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
	static CJump_Plate* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

