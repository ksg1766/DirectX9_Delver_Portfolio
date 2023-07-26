#include "..\Header\Beer.h"
#include "Export_Function.h"
#include "Player.h"

CBeer::CBeer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
{
}

CBeer::CBeer(const CBeer& rhs)
	: CItem(rhs)
{
}

CBeer::~CBeer()
{
	Free();
}

HRESULT CBeer::Ready_Object(_bool _Item)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_bWorldItem = _Item;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!Get_WorldItem())
	{
		m_pTransform->Scale(_vec3(-0.5f, 0.5f, 0.5f));
		CGameObject* pPlayer = SceneManager()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();
		
		m_pTransform->Translate(pPlayer->m_pTransform->m_vInfo[INFO_POS] + *dynamic_cast<CPlayer*>(pPlayer)->Get_LeftOffset());
	}
	else
		m_pTransform->Scale(_vec3(-0.5f, 0.5f, 0.5f));


	m_ItemID.eItemType = ITEMTYPE::ITEMTYPE_GENERALITEM;
	m_ItemID.eItemID = ITEMID::GENERAL_BEER;
	m_ItemID.iCount = 1;

	m_pBasicStat->Get_Stat()->fHealth = 1.f; // 1회 사용 느낌으로

	return S_OK;
}

_int CBeer::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipLeft());
	ITEMTYPEID ItemID = {};

	if (ItemType != nullptr)
		ItemID = ItemType->Get_ItemTag();

	if (ItemID.eItemID != ITEMID::GENERAL_BEER || !pPlayer->Get_ItemEquipLeft())
		return iExit;

	if (!Get_WorldItem())
	{
		CTransform* pPlayerTransform = pPlayer->m_pTransform;

		_vec3 vOffSet = 0.4f * -pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
		m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);

		_vec3 vLocalScale = m_pTransform->LocalScale();

		m_pTransform->Copy_RUL(pPlayerTransform->m_vInfo);
		for (_int i = 0; i < INFO_POS; ++i)
			m_pTransform->m_vInfo[i] *= *(((_float*)&vLocalScale) + i);
	}

	return iExit;
}

void CBeer::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(-0.5f, 0.5f, 0.5f));
//	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CBeer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

	if (!Get_WorldItem() && pPlayer != nullptr)
	{
		CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipLeft());

		ITEMTYPEID ItemID = {};

		if (ItemType != nullptr)
			ItemID = ItemType->Get_ItemTag();

		if (ItemID.eItemID == ITEMID::GENERAL_BEER && pPlayer->Get_ItemEquipLeft())
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

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBeer::Use_Beer(_float _use)
{
	{ m_pBasicStat->Take_Damage(_use); }
}

_float CBeer::Get_BeerCount()
{
	{ return m_pBasicStat->Get_Stat()->fHealth; }
}

HRESULT CBeer::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Beer"));
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

	if (!Get_WorldItem())
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());


		m_pTransform->Set_Parent(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform);
		m_pTransform->Copy_RUL(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo);

	}


	return S_OK;
}

void CBeer::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
}

void CBeer::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CBeer::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

CBeer* CBeer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item)
{
	CBeer* pInstance = new CBeer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Item)))
	{
		Safe_Release<CBeer*>(pInstance);

		MSG_BOX("Bear Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBeer::Free()
{
	__super::Free();
}
