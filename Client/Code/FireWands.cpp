#include "..\Header\FireWands.h"
#include "Export_Function.h"
#include "Player.h"
#include "EffectWand.h"


CFireWands::CFireWands(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CItem(pGraphicDev)
{
}

CFireWands::CFireWands(const CFireWands& rhs)
	: Engine::CItem(rhs)
{
}

CFireWands::~CFireWands()
{
	Free();
}

HRESULT CFireWands::Ready_Object(_bool _Item)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_bWorldItem = _Item;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!Get_WorldItem())
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

		CGameObject* pPlayer = SceneManager()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();
		//CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

		//m_pTransform->Translate(pPlayer->m_pTransform->m_vInfo[INFO_POS] + *dynamic_cast<CPlayer*>(pPlayer)->Get_Offset());

#pragma region ksg

		CTransform* pPlayerTransform = pPlayer->m_pTransform;

		_vec3 vOffSet = 0.7f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.4f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
		m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);

#pragma endregion ksg

		m_iAttackTick = 10;
	}
	else
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

		m_pCollider->
			InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

		m_pTransform->Translate(_vec3(0.0f, 2.f, 0.0f));
	}

	m_ItemID.eItemType = ITEMTYPE_WEAPONITEM;
	m_ItemID.eItemID = WEAPON_WAND3;
	m_ItemID.iCount = 1;

#pragma region FireWands
	m_pBasicStat->Get_Stat()->iDamageMax = 5.f;
	m_pBasicStat->Get_Stat()->iDamageMin = 2.f;
#pragma endregion
	return S_OK;
}

_int CFireWands::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);
	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());
	CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight());
	ITEMTYPEID ItemID = {};

	if (ItemType != nullptr)
		ItemID = ItemType->Get_ItemTag();

	if (ItemID.eItemID != ITEMID::WEAPON_WAND3 || !pPlayer->Get_ItemEquipRight())
		return iExit;

	if (!Get_WorldItem())
	{
		if (pPlayer->Get_Attack() && pPlayer != nullptr)
		{
			if (!m_bEffect)
			{
				m_bEffect = true;

				CGameObject* pGameObject = CEffectWand::Create(m_pGraphicDev);


				_vec3 vOffSet = -0.5f * m_pTransform->m_vInfo[INFO_RIGHT] + 1.4f * m_pTransform->m_vInfo[INFO_LOOK] + 0.4f * m_pTransform->m_vInfo[INFO_UP];
				pGameObject->m_pTransform->Translate(m_pTransform->m_vInfo[INFO_POS] + vOffSet);
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}

			if(m_iAttackTick > 0)
				m_pTransform->Translate(m_pTransform->m_vInfo[INFO_LOOK] * 0.1f);
			else
				m_pTransform->Translate(m_pTransform->m_vInfo[INFO_LOOK] * -0.1f);
			
			--m_iAttackTick;

			if (-9 == m_iAttackTick)
			{
				m_iAttackTick = 10;
				pPlayer->Set_Attack(false);

				CTransform* pPlayerTransform = pPlayer->m_pTransform;

				_vec3 vOffSet = 0.7f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.4f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
				m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);

				m_bEffect = false;
			}
		}
		else
		{
			CTransform* pPlayerTransform = pPlayer->m_pTransform;

			_vec3 vOffSet = 0.7f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.4f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
			m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);
		}
	}


	return iExit;
}

void CFireWands::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }


	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
}

void CFireWands::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

	if (!Get_WorldItem() && pPlayer != nullptr)
	{
		CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight());

		ITEMTYPEID ItemID = {};

		if (ItemType != nullptr)
			ItemID = ItemType->Get_ItemTag();

		if (ItemID.eItemID == ITEMID::WEAPON_WAND3 && pPlayer->Get_ItemEquipRight())
		{
			m_pTexture->Render_Texture();
			m_pBuffer->Render_Buffer();
		}
	}
	else
	{
		m_pTexture->Render_Texture();
		m_pBuffer->Render_Buffer();

#if _DEBUG
		m_pCollider->Render_Collider();
#endif
	}


}

HRESULT CFireWands::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_FireWands"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	if (!Get_WorldItem())
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
		
	
		m_pTransform->Set_Parent(SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform);
		m_pTransform->Copy_RUL(SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo);


		for (int i = 0; i < ID_END; ++i)
			for (auto& iter : m_mapComponent[i])
				iter.second->Init_Property(this);
		
	}
	else
	{
		//pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
		//NULL_CHECK_RETURN(pComponent, E_FAIL);
		//m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);


		for (int i = 0; i < ID_END; ++i)
			for (auto& iter : m_mapComponent[i])
				iter.second->Init_Property(this);
	}



	return S_OK;
}


void CFireWands::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
	// 몬스터거나 플레이어면 밀어내지않는다.
}

void CFireWands::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CFireWands::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

CFireWands* CFireWands::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item)
{
	CFireWands* pInstance = new CFireWands(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Item)))
	{
		Safe_Release<CFireWands*>(pInstance);

		MSG_BOX("Create FireWands Failed");
		return nullptr;
	}

	return pInstance;
}

void CFireWands::Free()
{
	__super::Free();
}
