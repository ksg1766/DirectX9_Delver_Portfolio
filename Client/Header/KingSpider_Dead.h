#pragma once
#include "State.h"
class CKingSpider_Dead :
    public CState
{
private:
	explicit CKingSpider_Dead();
	explicit CKingSpider_Dead(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKingSpider_Dead();


public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
	_float	m_fDelay;

public:
	static CKingSpider_Dead* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();

};

