#include "..\Header\FireWands.h"
#include "Export_Function.h"
#include "Player.h"


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

		m_pTransform->Translate(pPlayer->m_pTransform->m_vInfo[INFO_POS] + *dynamic_cast<CPlayer*>(pPlayer)->Get_Offset());

		m_fSignTime = 1.f;
	}
	else
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

	}

	m_ItemID.eItemType = ITEMTYPE_WEAPONITEM;
	m_ItemID.eItemID = WEAPON_WAND1;
	m_ItemID.iCount = 1;

	return S_OK;
}

_int CFireWands::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);
	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight());
	ITEMTYPEID ItemID = {};

	if (ItemType != nullptr)
		ItemID = ItemType->Get_ItemTag();

	if (ItemID.eItemID != ITEMID::WEAPON_WAND1 || !pPlayer->Get_ItemEquipRight())
		return iExit;



	if (!Get_WorldItem())
	{
		if (pPlayer->Get_Attack() && pPlayer != nullptr)
		{
			if (1.65f < D3DXVec3Length(&(m_pTransform->m_pParent->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS])))
			{
				m_fSignTime = -1.f;
				pPlayer->Set_Attack(false);
			}
			else if (1.45f > D3DXVec3Length(&(m_pTransform->m_pParent->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS])))
			{
				m_fSignTime = 1.f;
			}
			m_pTransform->Translate(m_pTransform->m_vInfo[INFO_LOOK] * m_fSignTime * 5.f * fTimeDelta);
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
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->GetInstance()->
		Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (!Get_WorldItem() && pPlayer != nullptr)
	{
		CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight());

		ITEMTYPEID ItemID = {};

		if (ItemType != nullptr)
			ItemID = ItemType->Get_ItemTag();

		if (ItemID.eItemID == ITEMID::WEAPON_WAND1 && pPlayer->Get_ItemEquipRight())
		{
			m_pTexture->Render_Texture();
			m_pBuffer->Render_Buffer();
		}
	}
	else
	{
		m_pTexture->Render_Texture();
		m_pBuffer->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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
		
	
		m_pTransform->Set_Parent(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform);
		m_pTransform->Copy_RUL(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo);


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

	if (!(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
	// ���Ͱų� �÷��̾�� �о���ʴ´�.
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