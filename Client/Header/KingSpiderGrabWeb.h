#pragma once
#include "Monster.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
class CBillBoard;
END

class CKingSpiderGrabWeb :
    public CMonster
{

private:
	explicit	CKingSpiderGrabWeb(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CKingSpiderGrabWeb(const CKingSpiderGrabWeb& rhs);
	virtual		~CKingSpiderGrabWeb();

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta)	override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;
public:
	virtual void	Init_Stat()override;
public:

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

	void			Set_Dir(_vec3 _vDir);

private:
	CCubeBf* m_pCubeBf;
	CTexture* m_pTexture = nullptr;

private:
	HRESULT		Add_Component(void);

private:
	_vec3		m_vDir;
	_vec3		m_vTargetPos;
	_float		m_vfDistance;
	_bool		m_bHit;
public:
	static CKingSpiderGrabWeb* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

