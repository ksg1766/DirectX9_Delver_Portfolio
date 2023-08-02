#pragma once
#include "State.h"
class CKingSpiderScream :
    public CState
{
private:
	explicit CKingSpiderScream();
	explicit CKingSpiderScream(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKingSpiderScream();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();
public:
	virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
public:
	static CKingSpiderScream* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	_bool		m_bScream;
	_bool		m_bSound;
	_float		m_fDelay;
private:
	virtual void Free();
};

