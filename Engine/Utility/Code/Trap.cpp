#include "..\..\Header\Trap.h"

CTrap::CTrap(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTrap::CTrap(const CTrap& rhs)
	: CGameObject(rhs), m_eTrapTag(rhs.m_eTrapTag)
{
}

CTrap::~CTrap()
{
}

HRESULT CTrap::Ready_Object()
{


	return S_OK;
}

_int CTrap::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CTrap::LateUpdate_Object()
{
}

void CTrap::Free(void)
{
	__super::Free();
}
