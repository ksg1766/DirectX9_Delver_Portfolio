#include "Monster.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMonster::CMonster(const CMonster& rhs)
	: CGameObject(rhs)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_Object()
{
	
	return S_OK;
}

_int CMonster::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CMonster::Free(void)
{
	__super::Free();
}
