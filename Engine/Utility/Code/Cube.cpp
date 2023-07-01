#include "Cube.h"

CCube::CCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CCube::CCube(const CCube& rhs)
	: CGameObject(rhs)
{
}

CCube::~CCube()
{
}

HRESULT CCube::Ready_Object()
{

	return S_OK;
}

_int CCube::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CCube::Free(void)
{
	__super::Free();
}
