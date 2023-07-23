#pragma once
#include "Monster.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

END

class CBossLostSoul :
    public CMonster
{
	enum SoulState { SOUL_NORMAL, SOUL_PARRY, SOUL_END };//기본, 패링당했을 때
private:
	explicit	CBossLostSoul(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBossLostSoul(const CBossLostSoul& rhs);
	virtual		~CBossLostSoul();

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta)	override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

public:
	virtual void	Init_Stat()								override;

public:
	//테스트용충돌코드
	void			Set_Target(_vec3 _vPos);

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	CRcTex*			m_pBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;
	CBillBoard*		m_pBillBoard = nullptr;
	_float			m_fFrame;
	_float			m_fTime;
	_float			m_fSpeed;
	_vec3			m_vTargetPos;
	_vec3			m_vDir;
	_bool			m_bHit;
	_bool			m_bParry;
	SoulState		m_eSoulState;
private:
	HRESULT		Add_Component(void);

public:
	static CBossLostSoul* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

