#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

END

class CBoss_Lightning :
    public CMonster
{
private:
	explicit	CBoss_Lightning(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBoss_Lightning(const CBoss_Lightning& rhs);
	virtual		~CBoss_Lightning();
public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta)	override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;
public:
	virtual void	Init_Stat()								override;
public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);
private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;
	_float			m_fTime;
	_float			m_fScale;
	_vec3			m_vDir[4];
	_bool			m_bHit;
private:
	HRESULT		Add_Component(void);

public:
	static CBoss_Lightning* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

