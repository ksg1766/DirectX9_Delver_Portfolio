#include "HpPotion.h"
#include "Export_Function.h"
#include "Player.h"

CHpPotion::CHpPotion(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
{
}

CHpPotion::CHpPotion(const CHpPotion& rhs)
	: CItem(rhs)
{
}

CHpPotion::~CHpPotion()
{
	Free();
}

HRESULT CHpPotion::Ready_Object(_bool _Item)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_bWorldItem = _Item;
	m_bHealOn = false;
	m_fHealingTick = 0.f;
	m_bUse = false;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	if (!Get_WorldItem())
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
		m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
			m_pTransform->LocalScale());
	}
	else
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

	m_ItemID.eItemType = ITEMTYPE::ITEMTYPE_POTIONITEM;
	m_ItemID.eItemID = ITEMID::EAT_POTION2;
	m_ItemID.iCount = 1;

#pragma region HpPotion
	m_pBasicStat->Get_Stat()->fMaxHP = 5.f;
	m_pBasicStat->Get_Stat()->fHP = 1.f;
#pragma endregion

	return S_OK;
}

_int CHpPotion::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);


	if (m_bHealOn)
		IsHealing(fTimeDelta);



	return iExit;
}

void CHpPotion::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
}

void CHpPotion::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();
}

HRESULT CHpPotion::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_HpPotion"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = static_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	if (Get_WorldItem())
	{
		pComponent = m_pBillBoard = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);
	}

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);



	return S_OK;
}

void CHpPotion::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
}

void CHpPotion::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CHpPotion::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CHpPotion::IsHealing(const _float& fTimeDelta)
{

	CPlayer& rPlayer = 
		*static_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	m_fHealingTick += fTimeDelta;

	if (m_fHealingTick > 1.f)
	{
		if (this->Get_ItemStat()->Get_Stat()->fMaxHP > 0)
		{

			rPlayer.Get_Stat()->Get_Stat()->fHP += this->Get_ItemStat()->Get_Stat()->fHP;

			if (rPlayer.Get_Stat()->Get_Stat()->fHP >= rPlayer.Get_Stat()->Get_Stat()->fMaxHP)
				rPlayer.Get_Stat()->Get_Stat()->fHP = rPlayer.Get_Stat()->Get_Stat()->fMaxHP;

			--this->Get_ItemStat()->Get_Stat()->fMaxHP;
		}
		else
			m_bHealOn = false;
	
		m_fHealingTick = 0.f;
	}



	CGameObject* pItem = rPlayer.Get_Inventory()->Get_IDItem(m_ItemID.eItemID);

	
	if (pItem != nullptr && m_bUse)
	{
		ITEMTYPEID	ItemType = static_cast<CItem*>(pItem)->Get_ItemTag();

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

		m_bUse = false;
	}
}

CHpPotion* CHpPotion::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item)
{
	CHpPotion* pInstance = new CHpPotion(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Item)))
	{
		Safe_Release<CHpPotion*>(pInstance);

		MSG_BOX("Bear Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CHpPotion::Free()
{
	__super::Free();
}
