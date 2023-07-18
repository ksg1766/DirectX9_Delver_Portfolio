#pragma once
#include "Monster.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;
END

class CProjectile_Trap_Arrow :
    public CMonster
{
private:
	explicit	CProjectile_Trap_Arrow(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CProjectile_Trap_Arrow(const CProjectile_Trap_Arrow& rhs);
	virtual		~CProjectile_Trap_Arrow();

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int Update_Object(const _float& fTimeDelta)	override;
	virtual void LateUpdate_Object(void)					override;
	virtual void Render_Object(void)						override;

public:
	virtual void Init_Stat()								override;

public:
	//테스트용충돌코드
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

	void				Set_Dir(_vec3 _vDir);
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;
	_float			m_fTime;
	_float			m_fFrame;
	_vec3 m_vTargetPos;
	_vec3 m_vDir;
private:
	HRESULT		Add_Component(void);

public:
	static CProjectile_Trap_Arrow* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

