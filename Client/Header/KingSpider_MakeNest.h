#pragma once
#include "State.h"
class CKingSpider_MakeNest :
    public CState
{
private:
	explicit CKingSpider_MakeNest();
	explicit CKingSpider_MakeNest(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKingSpider_MakeNest();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();
public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:

	_float	m_fDelay;
	_bool	m_bMakeNest;
	_vec3	m_vDir;
	_vec3	m_vTargetPos;
	_vec3	m_vSpawnPos[4];
public:
	static CKingSpider_MakeNest* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

