#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;

END

class CBoneGhost : public CMonster
{
private:
	explicit CBoneGhost(LPDIRECT3DDEVICE9 pGrapicDev);
	explicit CBoneGhost(const CBoneGhost& rhs);
	virtual ~CBoneGhost();

public:
	virtual HRESULT	Ready_Object()								override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object()							override;
	virtual void	Render_Object()								override;

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
	static CBoneGhost* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

