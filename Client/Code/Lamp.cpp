#include "..\Header\Lamp.h"
#include "Export_Function.h"
#include "Player.h"

CLamp::CLamp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
{
}

CLamp::CLamp(const CLamp& rhs)
	: CItem(rhs)
{
}

CLamp::~CLamp()
{
	Free();
}

HRESULT CLamp::Ready_Object(_bool _Item)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_bWorldItem = _Item;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!Get_WorldItem())
	{
		m_pTransform->Scale(_vec3(-0.3f, 0.3f, 0.3f));
		CGameObject* pPlayer = SceneManager()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();

		m_pTransform->Translate(pPlayer->m_pTransform->m_vInfo[INFO_POS] + *dynamic_cast<CPlayer*>(pPlayer)->Get_LeftOffset());

	}
	else
		m_pTransform->Scale(_vec3(-0.3f, 0.3f, 0.3f));


	m_ItemID.eItemType = ITEMTYPE::ITEMTYPE_GENERALITEM;
	m_ItemID.eItemID = ITEMID::GENERAL_LAMP;
	m_ItemID.iCount = 1;
	m_iMoveTick = 10;

	m_pBasicStat->Get_Stat()->fHealth = 1.f; // 1회 사용 느낌으로

	return S_OK;
}

_int CLamp::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipLeft());
	ITEMTYPEID ItemID = {};

	if (ItemType != nullptr)
		ItemID = ItemType->Get_ItemTag();

	if (ItemID.eItemID != ITEMID::GENERAL_LAMP || !pPlayer->Get_ItemEquipLeft())
		return iExit;


	if (!Get_WorldItem() && pPlayer != nullptr)
	{
		if (m_iMoveTick > 0)
			m_pTransform->Translate(m_pTransform->m_pParent->m_vInfo[INFO_UP] * 0.008f);
		else
			m_pTransform->Translate(m_pTransform->m_pParent->m_vInfo[INFO_UP] * -0.008f);

		--m_iMoveTick;

		if (-9 == m_iMoveTick)
			m_iMoveTick = 10;


		if(pPlayer->Get_StateMachine()->Get_State() == STATE::IDLE)
		{
			CTransform* pPlayerTransform = pPlayer->m_pTransform;

			_vec3 vOffSet = 0.55f * -pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.3f * pPlayerTransform->m_vInfo[INFO_UP];
			m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);
			m_iMoveTick = 0;
			//_vec3 vLocalScale = m_pTransform->LocalScale();

			//m_pTransform->Copy_RUL(pPlayerTransform->m_vInfo);
			//for (_int i = 0; i < INFO_POS; ++i)
			//	m_pTransform->m_vInfo[i] *= *(((_float*)&vLocalScale) + i);
		}

	
	}


	return iExit;
}

void CLamp::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(-0.3f, 0.3f, 0.3f));
	//	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CLamp::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

	if (!Get_WorldItem() && pPlayer != nullptr)
	{
		CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipLeft());

		ITEMTYPEID ItemID = {};

		if (ItemType != nullptr)
			ItemID = ItemType->Get_ItemTag();

		if (ItemID.eItemID == ITEMID::GENERAL_LAMP && pPlayer->Get_ItemEquipLeft())
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
#endif // _DEBUG
	}
}

HRESULT CLamp::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Lamp"));
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
	else
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

		m_pTransform->Set_Parent(pPlayer->m_pTransform);
		m_pTransform->Copy_RUL(pPlayer->m_pTransform->m_vInfo);
	}

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);


	return S_OK;
}

void CLamp::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
}

void CLamp::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CLamp::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

CLamp* CLamp::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item)
{
	CLamp* pInstance = new CLamp(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Item)))
	{
		Safe_Release<CLamp*>(pInstance);

		MSG_BOX("Bear Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLamp::Free()
{
	__super::Free();
}
