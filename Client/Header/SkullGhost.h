#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;

END

class CSkullGhost : public CMonster
{
private:
	explicit CSkullGhost(LPDIRECT3DDEVICE9 pGrapicDev);
	explicit CSkullGhost(const CSkullGhost& rhs);
	virtual ~CSkullGhost();

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
	CTexture*	 m_pTexture[static_cast<_uint>(STATE::STATE_END)] = {};
	CRcTex*		 m_pBuffer	 = nullptr;
	CAnimator*	 m_pAnimator = nullptr;


public:
	static CSkullGhost* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

