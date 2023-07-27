#pragma once
#include "State.h"

BEGIN(Engine)

END

class CWizard_Attack : public CState
{
private:
	explicit CWizard_Attack();
	explicit CWizard_Attack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWizard_Attack();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
	void Attack_Sound();

private:
	_vec3	m_vPrevPos;
	_bool	m_bIsAttack;

	_bool	m_bAttackTick;

private:

public:
	static CWizard_Attack* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

