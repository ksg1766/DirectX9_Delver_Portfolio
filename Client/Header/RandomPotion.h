#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

END

class CRandomPotion : public Engine::CItem
{
private:
	explicit CRandomPotion(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRandomPotion(const CRandomPotion& rhs);
	virtual ~CRandomPotion();

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
	void	Random_Usage(const _float& fTimeDelta);
	void	Set_RandomUse(_bool _fullheal) { m_bRandomUsage = _fullheal; }
	void	Set_Use(_bool _Use)			   { m_bUse = _Use; }
private:
	CRcTex*	  m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;
	_bool	  m_bRandomUsage;
	_bool	  m_bUse;
	_float	  m_fAddictionTick;

public:
	static CRandomPotion* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item);

private:
	virtual void Free() override;
};

