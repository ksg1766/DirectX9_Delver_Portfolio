#pragma once

#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CMonsterAI;

END

class CTerrain;

class CDungeonSpider : public CMonster
{
	enum class STATE
	{
		Roming,
		Jump,
	};

private:
	explicit CDungeonSpider(LPDIRECT3DDEVICE9 pGrapicDev);
	explicit CDungeonSpider(const CDungeonSpider& rhs);
	virtual ~CDungeonSpider();

public:
	virtual HRESULT	Ready_Object()								override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object()							override;
	virtual void	Render_Object()								override;
	
public:
	void		Set_Terrain(CTerrain* _pCurrentTerrain) { m_pTerrain = _pCurrentTerrain; }

	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	HRESULT		Add_Component();
	void		Jump(const _float& fTimeDelta);
	void		ForceHeight(_vec3 _vPos);

private:
	CRcTex*			m_pBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;
	CTerrain*		m_pTerrain = nullptr;
	CMonsterAI*		m_pAI = nullptr;

	_float		m_fFrame;
	_bool		m_bIsJumping;

	_vec3		m_vMonsterVelocity;
	_float		m_fJumpInitializeVelocity;

	_float		m_fChase;
	STATE		m_eState;

public:
	static CDungeonSpider* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

