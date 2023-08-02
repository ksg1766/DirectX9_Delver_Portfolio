#pragma once
#include "Monster.h"
BEGIN(Engine)
class CRcTex;
class CTexture;
class CBillBoard;
END

class CKingSpiderPoison :
    public CMonster
{
private:
	explicit	CKingSpiderPoison(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CKingSpiderPoison(const CKingSpiderPoison& rhs);
	virtual		~CKingSpiderPoison();

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

	void Set_DirParabola(_vec3 _vDir);
	void Set_DirStraight(_vec3 _vDir);

	
private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;

	_vec3	m_vDir;
	_bool	m_bHit;
	_bool	m_bStraight;
private:
	HRESULT		Add_Component(void);

public:
	static CKingSpiderPoison* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

