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

public:
	static CMonster_Move* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	void	Set_NewPos();
	void	Move_NewPos(const _float& fTimeDelta);
	_bool	Reached_Pos();
	

private:
	virtual void Free();
};

