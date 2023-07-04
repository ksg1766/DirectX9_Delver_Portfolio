#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CBasicStat :
    public CComponent
{
private:
	explicit CBasicStat();
	explicit CBasicStat(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBasicStat(const CBasicStat& rhs);
	virtual ~CBasicStat();

public:
	void			Take_Damage(_float _fAttack) { m_tBasicStat.fHealth -= _fAttack;  }

public:
	HRESULT			Ready_BasicStat();
	virtual _int	LateUpdate_Component(const _float& fTimeDelta);

protected:
	typedef struct tagBasicStat {
		_float	fHealth		= 4.f;
		_float	fMagic		= 4.f;
		_float	fSpeed		= 4.f;
		_float	fAttack		= 4.f;
		_float	fDeffense	= 4.f;
		_float	fAgility	= 4.f;
		//STATE	eState		= STATE::IDLE;
	}BASICSTAT;

	BASICSTAT m_tBasicStat;

public:
	static	CBasicStat*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
private:
	virtual void			Free();

};

END