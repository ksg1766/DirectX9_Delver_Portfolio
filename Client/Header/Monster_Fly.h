#pragma once

#include "State.h"

BEGIN(Engine)

END

class CMonster_Fly : public CState
{
private:
	explicit CMonster_Fly();
	explicit CMonster_Fly(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster_Fly();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE Key_Input(const _float& fTimeDelta) { return STATE(); }


private:
	void	Fly(const _float& fTimeDelta);
	void	Get_RandomDir(_vec3& vDir, const _vec3& _vCenter, const _float& fRange);
	void	Lerp_Move(_vec3& vDir, const _float& fTimeDelta);

private:
	_vec3	m_vSavePos;

	_float	m_fAttackDuration;
	_float	m_fReturnTime;
	_float  m_fLerpTime;

	_bool	m_bAttackCoolDown;
	_bool	m_bFirstCool;
	_bool	m_bCheck;
	_bool	m_bInSightMove;

public:
	static CMonster_Fly* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

