#pragma once
#include "State.h"


BEGIN(Engine)

END

class CPlayer_Attack : public CState
{
private:
	explicit CPlayer_Attack();
	explicit CPlayer_Attack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer_Attack();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta);

private:
	//_float fMaxDistance;	// 바라보는 시작지점에 최대 거리
	//_float fMoveSpeed;		// 공격 속도 
	//
	//_vec3	vStartPos;		// 플레이어가 바라보는 시작 지점.
	//_vec3	vTargetPos;		// 때릴 위치(Range)
	//_vec3	vCurrentPos;	// 현재 위치

public:
	static CPlayer_Attack* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

