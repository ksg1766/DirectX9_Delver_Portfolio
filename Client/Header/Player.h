#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CPlayerBf;
class CTransform;
class CStateMachine;
class CPlayerStat;
class CAnimator;

END

class CTerrain;
class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

	void			Set_Terrain(CTerrain* _pCurrentTerrain);

private:
	HRESULT			Add_Component(void);
	void			Key_Input(const _float& fTimeDelta);
	void			ForceHeight(_vec3 _vPos);

public:
	_vec3			Get_Offset() { return m_vOffset; }
	CPlayerStat*	Get_Stat()	 { return m_pStat; }

	_vec3			Set_Offset(_vec3 _vOffset) { m_vOffset = _vOffset; }

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	CPlayerBf*		m_pBuffer = nullptr;
	CStateMachine*	m_pStateMachine = nullptr;
	CPlayerStat*	m_pStat = nullptr;
	CAnimator*		m_pAnimator = nullptr;

	_float			m_fSpeed = 10.f;


	CTerrain*		m_pTerrain;

	_vec3			m_vOffset;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

