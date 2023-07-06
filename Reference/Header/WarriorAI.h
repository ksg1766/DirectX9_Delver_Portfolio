#pragma once

#include "MonsterAI.h"

BEGIN(Engine)

class ENGINE_DLL CWarriorAI : public CMonsterAI
{
private:
	explicit CWarriorAI();
	explicit CWarriorAI(LPDIRECT3DDEVICE9 pGrahicDev);
	explicit CWarriorAI(const CWarriorAI& rhs);
	virtual ~CWarriorAI();

public:
	HRESULT	 Ready_WarriorAI();
	virtual _int Update_Component(const _float& fTimeDelta);

public:
	void Set_NewTargetPosition();
	void Move_To_TargetPosition(const _float& fTimeDelta);
	_bool Reached_TargetPosition();

public:
	static CWarriorAI* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	_vec3			m_vTargetPos;


private:
	virtual void Free();
};

END

