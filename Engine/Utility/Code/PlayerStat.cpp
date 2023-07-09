#include "..\..\Header\PlayerStat.h"

CPlayerStat::CPlayerStat()
{
}

CPlayerStat::CPlayerStat(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CBasicStat(pGraphicDev)
{
}

CPlayerStat::CPlayerStat(const CPlayerStat& rhs)
	: Engine::CBasicStat(rhs)
{
}

CPlayerStat::~CPlayerStat()
{
}

HRESULT CPlayerStat::Ready_PlayerStat()
{
	m_tBasicStat.fHealth = 12.f;
	

	return S_OK;
}

_int CPlayerStat::Update_Component(const _float& fTimeDelta)
{
	return _int();
}

CPlayerStat* CPlayerStat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerStat* pStat = new CPlayerStat(pGraphicDev);

	if (FAILED(pStat->Ready_PlayerStat()))
	{
		Safe_Release<CPlayerStat*>(pStat);

		MSG_BOX("Create Stat Failed");
		return nullptr;
	}

	return pStat;
}

CComponent* CPlayerStat::Clone()
{
	return new CPlayerStat(*this);
}

void CPlayerStat::Free()
{
	__super::Free();
}
