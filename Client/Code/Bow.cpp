#include "stdafx.h"
#include "..\Header\Player.h"

#include "..\Header\Bow.h"
#include "Export_Function.h"
#include "EffectDamage.h"
#include "CameraManager.h"

static _int iCount = 0;

CBow::CBow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CItem(pGraphicDev)
{
}

CBow::CBow(const CBow& rhs)
	: Engine::CItem(rhs)
{
}

CBow::~CBow()
{
	Free();
}

HRESULT CBow::Ready_Object(_bool _Item)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_bWorldItem = _Item;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	/*m_pCollider->
		InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());*/

	if (!Get_WorldItem())
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

		CGameObject* pPlayer = SceneManager()->Get_Scene()->Get_MainPlayer();
		//CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTransform->m_pParent->Get_Host());

		// 밑으로 ksg가 주석 처리 함
		//m_pTransform->Copy_RUL(m_pTransform->m_pParent->m_vInfo);
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

		//m_pTransform->Translate(m_pTransform->m_pParent->m_vInfo[INFO_POS] + *dynamic_cast<CPlayer*>(pPlayer)->Get_Offset());

#pragma region ksg

		CTransform* pPlayerTransform = pPlayer->m_pTransform;

		_vec3 vOffSet = 0.6f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.6f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
		m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);

#pragma endregion ksg

		m_iAttackTick = 10;
		m_fChase = 0.f;
		m_fChase2 = 0.f;
		m_fAngle = 0.f;


		CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
			m_pTexture[_uint(STATE::IDLE)], STATE::IDLE, 0.5f, TRUE);
		m_pAnimator->Add_Animation(STATE::IDLE, pAnimation);
		pAnimation = CAnimation::Create(m_pGraphicDev,
			m_pTexture[_uint(STATE::ROMIMG)], STATE::ROMIMG, 3.f, TRUE);
		m_pAnimator->Add_Animation(STATE::ROMIMG, pAnimation);
		pAnimation = CAnimation::Create(m_pGraphicDev,
			m_pTexture[_uint(STATE::ATTACK)], STATE::ATTACK, 1.f, TRUE);
		m_pAnimator->Add_Animation(STATE::ATTACK, pAnimation);

		m_pAnimator->Set_Animation(STATE::IDLE);
	}
	else
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

		m_pCollider->
			InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

		m_pTransform->Translate(_vec3(0.0f, 2.f, 0.0f));
		//m_pCollider->
		//	InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	}


	// 타입 및 아이디 지정
	m_ItemID.eItemType = ITEMTYPE::ITEMTYPE_WEAPONITEM;
	m_ItemID.eItemID = WEAPON_BOW;
	m_ItemID.iCount = 1;


#pragma region Bow
	m_pBasicStat->Get_Stat()->iDamageMax = 2.f;
	m_pBasicStat->Get_Stat()->iDamageMin = 1.f;
#pragma endregion

	return S_OK;
}

_int CBow::Update_Object(const _float& fTimeDelta)
{
	if (m_bWorldItem) {
		Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	}
	else if (!m_bWorldItem) {
		Engine::Renderer()->Add_RenderGroup(RENDER_ITEM, this);
	}
	
	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());
	CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight());
	ITEMTYPEID ItemID = {};

	if (Get_WorldItem())
	{
		if (m_bDropAnItem)
		{
			DropanItem(pPlayer);

			m_pRigidBody->UseGravity(true);
			m_pRigidBody->Update_RigidBody(fTimeDelta);
		}
		else
			m_pRigidBody->UseGravity(false);
	}

	if (ItemType != nullptr)
		ItemID = ItemType->Get_ItemTag();

	if (ItemID.eItemID != ITEMID::WEAPON_BOW || !pPlayer->Get_ItemEquipRight())
		return iExit;

	if (!Get_WorldItem())
	{
		pPlayer = dynamic_cast<CPlayer*>(m_pTransform->m_pParent->Get_Host());
		_vec3 vUp = *D3DXVec3Cross(&vUp, &m_pTransform->m_pParent->m_vInfo[INFO_LOOK],
			&m_pTransform->m_pParent->m_vInfo[INFO_RIGHT]);
		_matrix matRot;
		_vec3 vLook = *D3DXVec3Cross(&vLook, &m_pTransform->m_pParent->m_vInfo[INFO_UP],
			&m_pTransform->m_pParent->m_vInfo[INFO_RIGHT]);

		if (pPlayer->Get_Attack())
		{
			if (m_iCount < 24.f)
			{
				++m_iCount;

				m_pTransform->RotateAround(m_pTransform->m_pParent->m_vInfo[INFO_POS], vUp, -0.01f);
				m_pTransform->RotateAround(m_pTransform->m_vInfo[INFO_POS], m_pTransform->m_vInfo[INFO_UP], 0.06f);

				CGameObject* pGameObject = CEffectDamage::Create(m_pGraphicDev);
				pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vInfo[INFO_POS].z));
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

				m_pAnimator->Set_Animation(STATE::ROMIMG);
			}

			if (m_iCount >= 24)
				m_pAnimator->Set_Animation(STATE::ATTACK);

			pPlayer->Set_Attack(false);
		}
		else
		{
			if (m_iCount > 0.f)
			{
				--m_iCount;
				m_pTransform->RotateAround(m_pTransform->m_pParent->m_vInfo[INFO_POS], vUp, 0.01f);
				m_pTransform->RotateAround(m_pTransform->m_vInfo[INFO_POS], m_pTransform->m_vInfo[INFO_UP], -0.06f);
			}
			else
			{
				CTransform* pPlayerTransform = pPlayer->m_pTransform;

				_vec3 vOffSet = 0.6f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.6f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
				m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);

				m_pAnimator->Set_Animation(STATE::IDLE);
				m_iMoveTick = 0;
			}
		}
		
		m_pAnimator->Update_Animator(fTimeDelta);
		return iExit;
	}
	else
	{
		m_pAnimator->Set_Animation(STATE::IDLE);
		return iExit;
	}

}

void CBow::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();

	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

	if(m_bDropAnItem)
	m_pCollider->SetCenterPos(m_pTransform->m_vInfo[INFO_POS] - _vec3(0.f, 0.3f, 0.f));
}

void CBow::Render_Object(void)
{
	if (!m_bWorldItem) {
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

	if (!Get_WorldItem() && pPlayer != nullptr)
	{
		CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight());

		ITEMTYPEID ItemID = {};

		if (ItemType != nullptr)
			ItemID = ItemType->Get_ItemTag();

		if (ItemID.eItemID == ITEMID::WEAPON_BOW && pPlayer->Get_ItemEquipRight())
		{
			m_pAnimator->Render_Animator();
			m_pBuffer->Render_Buffer();
		}
	}
	else
	{
		m_pTexture[(_uint)STATE::IDLE]->Render_Texture();
		m_pBuffer->Render_Buffer();

#if _DEBUG
		m_pCollider->Render_Collider();
#endif
	}

	if (!m_bWorldItem) {
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
}

HRESULT CBow::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture[(_uint)STATE::IDLE] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BowIdle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::ROMIMG] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BowRoming"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::ATTACK] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BowAttack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	//pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	if (!Get_WorldItem())
	{
		CTransform* pTransform = dynamic_cast<CTransform*>(SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform);

		m_pTransform->Set_Parent(pTransform);
		m_pTransform->Copy_RUL(m_pTransform->m_vInfo);

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

void CBow::OnCollisionEnter(CCollider* _pOther)
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
	// 몬스터거나 플레이어면 밀어내지않는다.
}

void CBow::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (Get_WorldItem())
	{
		__super::OnCollisionStay(_pOther);
		m_bDropAnItem = false;
	}
}

void CBow::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

CBow* CBow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item)
{
	CBow* pInstance = new CBow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Item)))
	{
		Safe_Release(pInstance);

		MSG_BOX("TempItem Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBow::Free()
{
	__super::Free();
}
