#pragma once

#include "MonsterAI.h"

BEGIN(Engine)

class ENGINE_DLL CSpiderAI : public CMonsterAI
{

private:
	explicit CSpiderAI();
	explicit CSpiderAI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSpiderAI(const CSpiderAI& rhs);
	virtual ~CSpiderAI();

public:
	HRESULT	Ready_SpiderAI();
	virtual _int Update_Component(const _float& fTimeDelta, const _vec3 _PlayerPos);

public:
	void	Jump(const _float& fTimeDelta);

public:
	_bool	Get_JumpState() { return m_bIsJumping; }

public:
	static CSpiderAI* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();


private:

	

private:
	virtual void Free();
};

END

