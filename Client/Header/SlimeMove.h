#pragma once

#include "State.h"

BEGIN(Engine)

END

class CSlimeMove : public CState
{
private:
	explicit CSlimeMove();
	explicit CSlimeMove(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSlimeMove();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

public:
	static CSlimeMove* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	void	Set_NewPos();
	void	Move_NewPos(const _float& fTimeDelta);
	_bool	Reached_Pos();

	void  Move_RandomPos(const _float& fTimeDelta);
	_vec3 Get_RandomDir(const _float& fTimeDelta);
	void  MoveTo_Pos(const _vec3& vTargetPos, const _float& fTimeDelta);

private:
	_bool	m_bJumCoolDown;
	_bool	m_bFirstCool;
	_float	m_fJumpCoolDuration;
	_float	m_fJumpCoolTimer;

	_vec3	m_vSavePos;
	STATE	m_ePrev;

private:
	virtual void Free();
};

