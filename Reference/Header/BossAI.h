#pragma once
#include "Component.h"


BEGIN(Engine)

class ENGINE_DLL CBossAI :
    public CComponent
{
private:
	explicit CBossAI();
	explicit CBossAI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBossAI(const CBossAI& rhs);
	virtual ~CBossAI();

public:
	void			Init_Property(_float _fSight, _float _fAttack, _vec3 _vRoamingCen, _float _RoamingRad, _float _fMaxTrace);

protected:
	void			Auto_Roaming();
	void			Auto_Attack();
	void			Back_Home();

	void			Chase_Player(const _float& fTimeDelta);

	_bool			Out_Of_Town();

public:
	HRESULT			Ready_MonsterAI();
	virtual _int	Update_Component(const _float& fTimeDelta);

protected:
	_float	m_fSightRange;
	_float	m_fAttackRange;

	_vec3	m_vRoamingCenter;
	_float	m_fRoamingRadius;

	_float	m_fMaxTraceRadius;

public:
	static	CBossAI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
private:
	virtual void			Free();

};

END