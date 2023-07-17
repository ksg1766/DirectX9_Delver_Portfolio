#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CAnimator;
class CTexture;
class CBasicStat;

END

class CTerrain;

class CFireBall : public CGameObject
{
private:
	explicit CFireBall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFireBall(const CFireBall& rhs);
	virtual ~CFireBall();

public:
	virtual HRESULT	Ready_Object(CTransform* pWeapon, CTransform* pOwner, _float _fSpeed);
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
	STATE	Get_State() {  return m_eState; }
	void	Set_State(STATE _eState) { m_eState = _eState; }

private:
	HRESULT	Add_Component();

private:
	CRcTex*			m_pBuffer	 =	 nullptr;
	CTerrain*		m_pTerrain	 =	 nullptr;
	CAnimator*		m_pAnimator  =	 nullptr;
	CBasicStat*		m_pBasicStat =	 nullptr;
	CTexture*		m_pTexture[(_uint)STATE::STATE_END] = {};

	_vec3			m_vDir;
	STATE			m_eState;

	CGameObject*    m_pEffect;

public:
	static CFireBall* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pWeapon, 
		CTransform* pOwner, _float _fSpeed);

private:
	virtual void Free() override;
};

