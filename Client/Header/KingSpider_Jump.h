#pragma once
#include "State.h"
class CKingSpider_Jump :
    public CState
{
private:
	explicit CKingSpider_Jump();
	explicit CKingSpider_Jump(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKingSpider_Jump();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();
public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
	_bool	m_bAppearTrigger;

	_float	m_fMoveDelay;
	_float	m_fFrameDelay;

	_vec3	m_vDir;
	_vec3	m_vTargetPos;

public:
	static CKingSpider_Jump* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

