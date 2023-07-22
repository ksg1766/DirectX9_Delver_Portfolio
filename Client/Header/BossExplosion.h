#pragma once
#include "Monster.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

END

class CBossExplosion :
    public CMonster
{
private:
	explicit	CBossExplosion(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBossExplosion(const CBossExplosion& rhs);
	virtual		~CBossExplosion();

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

public:
	virtual void	Init_Stat()								override;

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
	virtual void	Set_StartPos(_vec3 _vec);
	virtual void	Set_StartPosY(float _fY);
	void			Set_Scale(_float _fSclae);
private:
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;
	_float		m_fFrame;
	_float		m_fSclae;
	_uint		m_iCount;
	_bool		m_bHit;
private:
	HRESULT		Add_Component(void);

public:
	static CBossExplosion* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

