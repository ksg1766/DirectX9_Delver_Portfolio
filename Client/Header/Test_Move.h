#pragma once

#include "State.h"

BEGIN(Engine)

END

class CTest_Move : public CState
{
private:
	explicit CTest_Move();
	explicit CTest_Move(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTest_Move();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

public:
	static CTest_Move* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	_bool	m_bJumCoolDown;
	_bool	m_bFirstCool;
	_float	m_fJumpCoolDuration;
	_float	m_fJumpCoolTimer;

	_vec3	m_vSavePos;
	_vec3	m_vReverseDir;
	STATE	m_ePrev;

	_float	m_fAmplitude;
	_float	m_fTime;

private:
	virtual void Free();
};

