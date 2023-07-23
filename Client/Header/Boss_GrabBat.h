#pragma once
#include "Monster.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
END
class CBoss_GrabBat :
    public CMonster
{
private:
	explicit	CBoss_GrabBat(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBoss_GrabBat(const CBoss_GrabBat& rhs);
	virtual		~CBoss_GrabBat();

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int Update_Object(const _float& fTimeDelta)	override;
	virtual void LateUpdate_Object(void)					override;
	virtual void Render_Object(void)						override;

public:
	virtual void	Init_Stat()								override;

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);
private:
	HRESULT		Add_Component(void);
	void		PickingBat();
private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	_vec3		m_vDir;
	_vec3		m_vOffset;
	_float		m_fFrame;
	_float		m_fDuration;
	_float		m_fHitCool;
	_bool		m_bHit;
	_bool		m_bGrab;
	_int		m_iHp;
public:
	static CBoss_GrabBat* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

