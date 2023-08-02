#include "stdafx.h"
#include "SoundManager.h"
#include "Orb.h"
#include "Export_Function.h"
#include "Player.h" 


COrb::COrb(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CItem(pGraphicDev)
{
}

COrb::COrb(const COrb& rhs)
	: Engine::CItem(rhs)
{
}

COrb::~COrb()
{
	Free();
}

HRESULT COrb::Ready_Object(_bool _Item)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_bWorldItem = _Item;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	if (!Get_WorldItem())
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

		CGameObject* pPlayer = SceneManager()->Get_Scene()->Get_MainPlayer();

		CTransform* pPlayerTransform = pPlayer->m_pTransform;

		_vec3 vOffSet = 0.6f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.6f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
		m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);
		//m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		//	m_pTransform->LocalScale());
	}
	else
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

		m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

		m_pTransform->Translate(_vec3(0.0f, 2.f, 0.0f));
	}

	m_ItemID.eItemType = ITEMTYPE::ITEMTYPE_WEAPONITEM;
	m_ItemID.eItemID = ITEMID::QUEST_ORB;
	m_ItemID.iCount = 1;
	m_iMoveTick = 10;


	return S_OK;
}

_int COrb::Update_Object(const _float& fTimeDelta)
{
	if (m_bWorldItem) {
		Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	}
	else if (!m_bWorldItem) {
		Engine::Renderer()->Add_RenderGroup(RENDER_ITEM, this);
	}

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();
	CItem* ItemType = dynamic_cast<CItem*>(rPlayer.Get_CurrentEquipRight());
	ITEMTYPEID	ItemID = {};

	if (Get_WorldItem())
	{
		if (m_bDropAnItem)
		{
			DropanItem(&rPlayer);

			m_pRigidBody->UseGravity(true);
			m_pRigidBody->Update_RigidBody(fTimeDelta);
		}
		else
			m_pRigidBody->UseGravity(false);
	}


	if (ItemType != nullptr)
		ItemID = ItemType->Get_ItemTag();

	if (ItemID.eItemID != ITEMID::QUEST_ORB || !rPlayer.Get_ItemEquipRight())
		return iExit;

	if (!Get_WorldItem())
	{
		m_pRigidBody->UseGravity(false);

		if (rPlayer.Get_StateMachine()->Get_State() == STATE::ROMIMG && !rPlayer.IsJump())
		{
			if (m_iMoveTick > 0)
				m_pTransform->Translate(m_pTransform->m_vInfo[INFO_UP] * 0.01f);
			else
				m_pTransform->Translate(m_pTransform->m_vInfo[INFO_UP] * -0.01f);

			--m_iMoveTick;

			if (-9 == m_iMoveTick)
				m_iMoveTick = 10;

		}
		else
		{
			CTransform* pPlayerTransform = rPlayer.m_pTransform;

			_vec3 vOffSet = 0.6f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.6f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
			m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);

			m_iMoveTick = 0;
		}
	}


	return iExit;
}

void COrb::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(0.2f, 0.2f, 0.2f));

	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

	if (m_bDropAnItem)
		m_pCollider->SetCenterPos(m_pTransform->m_vInfo[INFO_POS] - _vec3(0.f, 0.3f, 0.f));

}


void COrb::Render_Object(void)
{
	if (!m_bWorldItem) {
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	}

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

		if (ItemID.eItemID == ITEMID::QUEST_ORB && pPlayer->Get_ItemEquipRight())
		{
			m_pTexture->Render_Texture(m_iCurrentImage);
			m_pBuffer->Render_Buffer();
		}
	}
	else
	{
		m_pTexture->Render_Texture(m_iCurrentImage);
		m_pBuffer->Render_Buffer();

#if _DEBUG
		m_pCollider->Render_Collider();
#endif
	}

	if (!m_bWorldItem) {
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
}

HRESULT COrb::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_itemUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	if (!Get_WorldItem())
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

		m_pTransform->Set_Parent(pPlayer->m_pTransform);
		m_pTransform->Copy_RUL(SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo);

		for (int i = 0; i < ID_END; ++i)
			for (auto& iter : m_mapComponent[i])
				iter.second->Init_Property(this);
	}
	else
	{
		pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

		for (int i = 0; i < ID_END; ++i)
			for (auto& iter : m_mapComponent[i])
				iter.second->Init_Property(this);
	}


	return S_OK;
}

void COrb::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);


	if (Get_WorldItem())
	{
		__super::OnCollisionEnter(_pOther);
		m_bDropAnItem = false;
	}

}

void COrb::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (Get_WorldItem())
	{
		__super::OnCollisionStay(_pOther);
		m_bDropAnItem = false;
	}
}

void COrb::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

}

COrb* COrb::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item)
{
	COrb* pInstance = new COrb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Item)))
	{
		Safe_Release(pInstance);

		MSG_BOX("TempItem Create Failed");
		return nullptr;
	}

	return pInstance;
}

void COrb::Free()
{
	__super::Free();
}