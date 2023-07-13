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
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	CRcTex*		 m_pBuffer = nullptr;
	CTexture*	 m_pTexture = nullptr;
	
	_float		 m_fSpeed;
public:
	static CArrow* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* Weapon, 
		CTransform* pOwner, _float _fSpeed);

private:
	virtual void Free() override;
};

