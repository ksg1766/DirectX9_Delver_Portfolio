#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CArrow : public Engine::CItem
{
private:
	explicit CArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CArrow(const CArrow& rhs);
	virtual ~CArrow();

public:
	virtual HRESULT Ready_Object(CTransform* Weapon, CTransform* pOwner, _float _fSpeed = 1.f);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT			Add_Component(void);

public:
	void	Set_State(STATE _eState) { m_eState = _eState; }
	STATE	Get_State() { return m_eState; }
public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	CRcTex*		 m_pBuffer = nullptr;
	CTexture*	 m_pTexture = nullptr;
	CTransform*  m_pPlayerTransform = nullptr;

	_vec3		 m_vDir;
	_float		 m_fSpeed;
	STATE		 m_eState;
public:
	static CArrow* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* Weapon, 
		CTransform* pOwner, _float _fSpeed);

private:
	virtual void Free() override;
};

