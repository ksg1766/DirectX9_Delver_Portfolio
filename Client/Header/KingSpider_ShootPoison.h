#pragma once
#include "State.h"
class CKingSpider_ShootPoison :
    public CState
{
private:
	explicit CKingSpider_ShootPoison();
	explicit CKingSpider_ShootPoison(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKingSpider_ShootPoison();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();
public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:


	_float	m_fDelay;
	_int	m_iSkillCount;
	_vec3	m_vDir;

public:
	static CKingSpider_ShootPoison* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

