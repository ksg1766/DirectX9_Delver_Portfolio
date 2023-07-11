#pragma once

#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CStateMachine;
class CBasicStat;
class CAnimator;

END

class CTerrain;

class CBat : public CMonster
{
private:
	explicit CBat(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBat(const CBat& rhs);
	virtual ~CBat();

public:
	virtual HRESULT	Ready_Object()								override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object()							override;
	virtual void	Render_Object()								override;

public:
	void		ForceHeight(_vec3 _vPos);
	void		Set_Terrain(CTerrain* _pCurrentTerrain) { m_pTerrain = _pCurrentTerrain; }

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	CRcTex*			m_pBuffer = nullptr;
	CTexture*		m_pTexture[(_uint)STATE::STATE_END] = {};
	CTerrain*		m_pTerrain = nullptr;
	CAnimator*		m_pAnimator = nullptr;

	_float m_fFrame;
	
private:
	HRESULT		Add_Component(void);


public:
	static CBat* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

