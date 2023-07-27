#pragma once

#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;

END

class CTerrain;

class CSlime : public CMonster
{
private:
	explicit CSlime(LPDIRECT3DDEVICE9 pGrapicDev);
	explicit CSlime(const CSlime& rhs);
	virtual ~CSlime();

public:
	virtual HRESULT	Ready_Object()								override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object()							override;
	virtual void	Render_Object()								override;

public:
	virtual void	Init_Stat()									override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	HRESULT		Add_Component();

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[static_cast<_uint>(STATE::STATE_END)] = {};
	CAnimator* m_pAnimator = nullptr;


	_bool		m_bAttackTick;
	_bool		m_bSearch = false;
	_float		m_fFrame;

	STATE		m_eState;

public:
	static CSlime* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

