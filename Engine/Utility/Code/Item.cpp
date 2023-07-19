#include "../../Header/Item.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	m_eObjectTag		= OBJECTTAG::ITEM;
	m_ItemID.eItemType	= ITEMTYPE::ITEMTYPE_END;
	m_ItemID.eItemID	= ITEMID::ITEMID_END;
	m_ItemID.iCount		= 1;
	m_bWorldItem		= true;
}

CItem::CItem(const CItem& rhs)
	: CGameObject(rhs)
{
	m_eObjectTag		= rhs.m_eObjectTag;
	m_ItemID.eItemType	= rhs.m_ItemID.eItemType;
	m_ItemID.eItemID	= rhs.m_ItemID.eItemID;
	m_ItemID.iCount		= rhs.m_ItemID.iCount;
	m_bWorldItem		= rhs.m_bWorldItem;
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