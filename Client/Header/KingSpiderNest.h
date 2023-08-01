#pragma once
#include "Monster.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CBillBoard;
END

class CKingSpiderNest :
    public CMonster
{
private:
	explicit	CKingSpiderNest(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CKingSpiderNest(const CKingSpiderNest& rhs);
	virtual		~CKingSpiderNest();

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

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;

	_float		m_fScale;
	_float		m_fDelay;
	_float		m_fHitCool;
	_bool		m_bSpawn;
	_bool		m_bMaxSize;
	_bool		m_bHit;
	_vec3		m_vSpawnPos[4];
	_int		m_iHp;
private:
	HRESULT		Add_Component(void);

public:
	static CKingSpiderNest* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

