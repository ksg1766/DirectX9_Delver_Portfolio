#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CHolyWater : public Engine::CItem
{
private:
	explicit CHolyWater(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHolyWater(const CHolyWater& rhs);
	virtual ~CHolyWater();

public:
	virtual HRESULT Ready_Object(_bool _Item);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;


private:
	HRESULT			Add_Component(void);

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
	void	FullHealing();
	void	Set_Heal(_bool _fullheal)	{ m_bFullHealOn = _fullheal; }

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	_bool	  m_bFullHealOn;

public:
	static CHolyWater* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item);

private:
	virtual void Free() override;
};

