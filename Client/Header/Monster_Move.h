#pragma once

#include "State.h"

BEGIN(Engine)

END

class CMonster_Move : public CState
{
private:
	explicit CMonster_Move();
	explicit CMonster_Move(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster_Move();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
	void Move_Sound();

public:
	static CMonster_Move* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	void Get_RandomDir(_vec3& vDir, const _vec3& _vCenter, const _float& fRange);
	void Lerp_Move(_vec3& vDir, const _float& fTimeDelta);

private:
	_float m_fDistance;
	_float m_fAngle;
	_float m_fChase;
	_float m_fSpeed;
	_float m_fRandomRange;
	_float m_fAttackCool;
	_float m_fReturnTime;




	_float m_fLerpTime;
	_vec3  m_vSavePos;

	_bool  m_bCheck;
	_bool  m_bInSightMove;

private:
	virtual void Free();
};

