#pragma once
#include "Monster.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CAnimator;
END

class CKingSpider :
    public CMonster
{
private:
	explicit CKingSpider(LPDIRECT3DDEVICE9 pGrapicDev);
	explicit CKingSpider(const CKingSpider& rhs);
	virtual ~CKingSpider();


public:
	virtual HRESULT	Ready_Object()								override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object()							override;
	virtual void	Render_Object()								override;

public:
	virtual void	Init_Stat()override;;
public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

	BOSSPHASE			Get_Phase() { return m_ePhase; }
	void				Set_Phase(BOSSPHASE _ePhase) {m_ePhase = _ePhase;}
private:
	HRESULT		Add_Component();

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[static_cast<_uint>(STATE::STATE_END)] = {};
	CAnimator* m_pAnimator = nullptr;

	BOSSPHASE	m_ePhase;

public:
	static CKingSpider* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free() override;
};

