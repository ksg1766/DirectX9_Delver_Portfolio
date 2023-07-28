#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END

//CFireWands

class CFireWands : public Engine::CItem
{
private:
	explicit CFireWands(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFireWands(const CFireWands& rhs);
	virtual ~CFireWands();

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
private:
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture = nullptr;

	_int		m_iAttackTick;
	_int		m_iMoveTick;

	_bool       m_bEffect = false;

public:
	static CFireWands* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item);

private:
	virtual void Free() override;
};

