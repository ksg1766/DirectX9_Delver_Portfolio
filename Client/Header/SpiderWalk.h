#pragma once
#include "State.h"

BEGIN(Engine)

END

class CSpiderWalk : public CState
{
private:
	explicit CSpiderWalk();
	explicit CSpiderWalk(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpiderWalk();

public:
	virtual HRESULT Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual	STATE		Key_Input(const _float& fTimeDelta) { return STATE(); }

public:
	static CSpiderWalk* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

