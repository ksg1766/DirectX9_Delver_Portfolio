#include "..\..\Header\BasicStat.h"

CBasicStat::CBasicStat()
{
}

CBasicStat::CBasicStat(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CBasicStat::CBasicStat(const CBasicStat& rhs)
	:CComponent(rhs)
{
}

CBasicStat::~CBasicStat()
{
}

HRESULT CBasicStat::Ready_BasicStat()
{

    return S_OK;
}

_int CBasicStat::LateUpdate_Component(const _float& fTimeDelta)
{
    return _int();
}

CBasicStat* CBasicStat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBasicStat* pInstance = new CBasicStat(pGraphicDev);

	if (FAILED(pInstance->Ready_BasicStat()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BasicStat Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CBasicStat::Clone(void)
{
	return new CBasicStat(*this);
}

void CBasicStat::Free()
{
	CComponent::Free();
}
