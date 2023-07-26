#pragma once
#include "State.h"

BEGIN(Engine)

END

class CWarror_Attack : public CState
{
private:
	explicit CWarror_Attack();
	explicit CWarror_Attack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWarror_Attack();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
	void	Attack_Sound();

private:
	_vec3	m_vPrevPos;
	_bool	m_bIsAttack;
	_float	m_fSpeed;
	_float	m_fTime;

	_bool	m_bAttackTick;
	_bool	m_bAttackTick2;

private:

public:
	static CWarror_Attack* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

