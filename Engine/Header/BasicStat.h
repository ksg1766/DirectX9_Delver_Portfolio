#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CBasicStat :
    public CComponent
{
protected:
	explicit CBasicStat();
	explicit CBasicStat(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBasicStat(const CBasicStat& rhs);
	virtual ~CBasicStat();
	
public:
	BASICSTAT* Get_Stat()		{ return &m_tBasicStat; }

public:
	void			Take_Damage(_float _fAttack) { m_tBasicStat.fHealth -= _fAttack;  }

public:
	HRESULT			Ready_BasicStat();
	virtual _int	LateUpdate_Component(const _float& fTimeDelta);

protected:
	BASICSTAT m_tBasicStat;

public:
	static	CBasicStat*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
protected:
	virtual void			Free();

};

END