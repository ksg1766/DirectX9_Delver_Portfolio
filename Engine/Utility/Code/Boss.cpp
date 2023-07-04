#include "../../Header/Boss.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CBoss::CBoss(const CBoss& rhs)
	: CMonster(rhs)
{
}

CBoss::~CBoss()
{
}

HRESULT CBoss::Ready_Object()
{

	return S_OK;
}

_int CBoss::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CBoss::Free(void)
{
	__super::Free();
}
