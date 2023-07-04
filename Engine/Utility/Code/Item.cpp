#include "../../Header/Item.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CItem::CItem(const CItem& rhs)
	: CGameObject(rhs)
{
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_Object()
{

	return S_OK;
}

_int CItem::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CItem::Free(void)
{
	__super::Free();
}