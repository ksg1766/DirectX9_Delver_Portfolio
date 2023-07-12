#pragma once
#include "State.h"

BEGIN(Engine)

END

class CSlimeAttack : public CState
{
private:
	explicit CSlimeAttack();
	explicit CSlimeAttack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSlimeAttack();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
	_vec3	m_vPrevPos;
	_bool	m_bIsAttack;
	_float	m_fSpeed;

	_bool	m_bAttackTick;

private:

public:
	static CSlimeAttack* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

