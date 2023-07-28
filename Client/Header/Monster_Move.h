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


private:
	void Move_RandomPos(const _float& fTimeDelta);
	void MoveTo_Pos(const _vec3& vTargetPos, const _float& fTimeDelta);
	_vec3 Get_RandomDir(const _float& fTimeDelta);
public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
	void Move_Sound();
public:
	static CMonster_Move* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	_float RandomRange(_float _Min, _float _Max)
	{
		_float fRandom = static_cast<_float>(rand()) / static_cast<_float>(RAND_MAX);
		return _Min + fRandom * (_Max - _Min);
	}

private:
	_float m_fDistance;
	_float m_fAngle;
	_float m_fChase;
	_float m_fSpeed;
	_float m_fRandomRange;
	_float m_fAttackCool;

	_vec3  m_vSavePos;

	_bool  m_bCheck;

private:
	virtual void Free();
};

