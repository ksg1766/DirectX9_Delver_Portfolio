#pragma once
#include "Monster.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

END

class CBossProjectile :
    public CMonster
{
private:
	explicit	CBossProjectile(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBossProjectile(const CBossProjectile& rhs);
	virtual		~CBossProjectile();

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
	CGameObject*	m_pEffect;
	_float			m_fFrame;
	_float			m_fTime;
	_float			m_fSpeed;
	_vec3			m_vTargetPos;
	_vec3			m_vDir;
	_bool			m_bHit;
private:
	HRESULT		Add_Component(void);

public:
	static CBossProjectile* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

