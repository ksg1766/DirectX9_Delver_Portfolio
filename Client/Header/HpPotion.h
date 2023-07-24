#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

END

class CHpPotion : public Engine::CItem
{
private:
	explicit CHpPotion(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHpPotion(const CHpPotion& rhs);
	virtual ~CHpPotion();

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
	void	IsHealing(const _float& fTimeDelta);
	void	Set_Heal(_bool _Heal) { m_bHealOn = _Heal; }
	void	Set_Use(_bool _Use) { m_bUse = _Use; }
private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;
	_bool	m_bHealOn;
	_bool	m_bUse;
	_float	m_fHealingTick;
public:
	static CHpPotion* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item);

private:
	virtual void Free() override;
};

