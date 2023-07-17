#pragma once

#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;

END

class CTerrain;

class CGhost_Bullet : public CMonster
{
private:
	explicit CGhost_Bullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGhost_Bullet(const CGhost_Bullet& rhs);
	virtual ~CGhost_Bullet();

public:
	virtual HRESULT	Ready_Object(CTransform* pOwner, _float _fSpeed, _vec3 _vOffset);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object() override;
	virtual void	Render_Object() override;

public:
	void		ForceHeight(_vec3 _vPos);
	void		Set_Terrain(CTerrain* _pCurrentTerrain) { m_pTerrain = _pCurrentTerrain; }

	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

public:

private:
	HRESULT	Add_Component();

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[(_uint)STATE::STATE_END] = {};
	CTerrain* m_pTerrain = nullptr;
	CAnimator* m_pAnimator = nullptr;

	_float m_fFrame;
	_float m_fSpeed;
	_bool  m_bIsAttack;
	_bool  m_bCheck;
	_vec3  m_vPrevPos;
	_vec3  m_vInit;

public:
	static CGhost_Bullet* Create(
		LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pOwner, _float _fSpeed, _vec3 _vOffset = _vec3(0.f, 0.f, 0.f));

private:
	virtual void Free() override;
};



