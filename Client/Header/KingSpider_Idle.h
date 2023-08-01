#pragma once
#include "State.h"
class CKingSpider_Idle :
    public CState
{
private:
	explicit CKingSpider_Idle();
	explicit CKingSpider_Idle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKingSpider_Idle();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();
public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
public:
	static CKingSpider_Idle* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

	void	Shoot_Web();
	void	Shoot_Poison();
	void	Earthquake();
	void	Make_Nest();
private:
	_bool		m_fJumpAttack;
	_float		m_fDelay;
	_int		m_iSkillCount;
	_vec3		m_vFogPos[8];
private:
	virtual void Free();
};

