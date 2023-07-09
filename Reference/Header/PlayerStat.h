#pragma once
#include "BasicStat.h"

BEGIN(Engine)

class ENGINE_DLL CPlayerStat : public CBasicStat
{
private:
	explicit CPlayerStat();
	explicit CPlayerStat(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerStat(const CPlayerStat& rhs);
	virtual ~CPlayerStat();

public:
	HRESULT	Ready_PlayerStat();
	virtual _int Update_Component(const _float& fTimeDelta);
	
public:
	static CPlayerStat* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void Free();
};

END