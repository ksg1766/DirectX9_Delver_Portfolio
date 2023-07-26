#include "HolyWater.h"
#include "Export_Function.h"
#include "Player.h"

CHolyWater::CHolyWater(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
{
}

CHolyWater::CHolyWater(const CHolyWater& rhs)
	: CItem(rhs)
{
}

CHolyWater::~CHolyWater()
{
	Free();
}

HRESULT CHolyWater::Ready_Object(_bool _Item)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_bWorldItem = _Item;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!Get_WorldItem())
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
		m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
			m_pTransform->LocalScale());

		CGameObject* pPlayer = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();

		if (pPlayer != nullptr)
		{
#pragma region HpPotion
			m_pBasicStat->Get_Stat()->fMaxHP = dynamic_cast<CPlayer*>(pPlayer)->Get_Stat()->Get_Stat()->fMaxHP;
#pragma endregion
		}
	}
	else
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
	}


	m_ItemID.eItemType = ITEMTYPE::ITEMTYPE_POTIONITEM;
	m_ItemID.eItemID = ITEMID::EAT_POTION7;
	m_ItemID.iCount = 1;

	m_bFullHealOn = false;

	return S_OK;
}

_int CHolyWater::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	if (m_bFullHealOn)
	{
		FullHealing();
	}

	return iExit;
}

void CHolyWater::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
	//	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CHolyWater::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif // _DEBUG

}

HRESULT CHolyWater::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_HolyWater"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	if (Get_WorldItem())
	{
		pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);
	}

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);


	return S_OK;
}

void CHolyWater::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
}

void CHolyWater::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CHolyWater::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CHolyWater::FullHealing()
{
	CPlayer& rPlayer =
		*dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	rPlayer.Get_Stat()->Get_Stat()->fHP = m_pBasicStat->Get_Stat()->fMaxHP;

	m_bFullHealOn = false;

	CGameObject* pItem = rPlayer.Get_Inventory()->Get_IDItem(m_ItemID.eItemID);

	if (pItem != nullptr)
	{
		ITEMTYPEID	ItemType = dynamic_cast<CItem*>(pItem)->Get_ItemTag();

		if (ItemType.iCount == 1)
		{
			rPlayer.Get_Inventory()->delete_FindItem(ItemType);
			Engine::UIManager()->Delete_FindItemUI(ItemType);
		}
		else if (ItemType.iCount > 1)
		{
			ItemType.iCount = 1;

			rPlayer.Get_Inventory()->delete_FindItem(ItemType);

			//아이템 UI 내부에서도 해당 아이템을 찾아 삭제.
			Engine::UIManager()->Delete_FindItemUI(ItemType);
		}
	}
}

CHolyWater* CHolyWater::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item)
{
	CHolyWater* pInstance = new CHolyWater(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Item)))
	{
		Safe_Release<CHolyWater*>(pInstance);

		MSG_BOX("Bear Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CHolyWater::Free()
{
	__super::Free();
}
