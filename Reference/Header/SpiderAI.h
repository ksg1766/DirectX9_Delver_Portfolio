#pragma once

#include "MonsterAI.h"

BEGIN(Engine)

class ENGINE_DLL CSpiderAI : public CMonsterAI
{
	enum class STATE
	{
		Roming,
		Jump,
	};


private:
	explicit CSpiderAI();
	explicit CSpiderAI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSpiderAI(const CSpiderAI& rhs);
	virtual ~CSpiderAI();

public:
	HRESULT	Ready_SpiderAI();
	virtual _int Update_Component(const _float& fTimeDelta);

public:
	void	Jump(const _float& fTimeDelta);

public:
	static CSpiderAI* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();


private:
	_float m_fChase;
	_float m_fJumpInitializeVelocity;


	_bool  m_bIsJumping;
	STATE  m_eState;

private:
	virtual void Free();
};

END

