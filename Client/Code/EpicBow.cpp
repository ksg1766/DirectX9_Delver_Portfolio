#include "stdafx.h"
#include "..\Header\Player.h"

#include "..\Header\EpicBow.h"
#include "Export_Function.h"
#include "EffectDamage.h"

static _int iCount = 0;

CEpicBow::CEpicBow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CItem(pGraphicDev)
{
}

CEpicBow::CEpicBow(const CEpicBow& rhs)
	: Engine::CItem(rhs)
{
}

CEpicBow::~CEpicBow()
{
	Free();
}

HRESULT CEpicBow::Ready_Object(_bool _Item)
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

		_vec3 vOffSet = 0.7f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.4f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
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
			m_pTexture[_uint(STATE::ROMIMG)], STATE::ROMIMG, 2.f, TRUE);
		m_pAnimator->Add_Animation(STATE::ROMIMG, pAnimation);
		pAnimation = CAnimation::Create(m_pGraphicDev,
			m_pTexture[_uint(STATE::ATTACK)], STATE::ATTACK, 5.f, TRUE);
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
	m_ItemID.eItemID = WEAPON_EPICBOW;
	m_ItemID.iCount = 1;


#pragma region EpicBow
	m_pBasicStat->Get_Stat()->iDamageMax = 6.f;
	m_pBasicStat->Get_Stat()->iDamageMin = 4.f;
#pragma endregion

	return S_OK;
}

_int CEpicBow::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());
	CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight());
	ITEMTYPEID ItemID = {};

	if (ItemType != nullptr)
		ItemID = ItemType->Get_ItemTag();

	if (ItemID.eItemID != ITEMID::WEAPON_EPICBOW || !pPlayer->Get_ItemEquipRight())
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
			m_pAnimator->Get_Animation()->Set_Loop(true);

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

				_vec3 vOffSet = 0.7f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.4f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
				m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);

				m_pAnimator->Set_Animation(STATE::IDLE);
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

void CEpicBow::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
}

void CEpicBow::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

	if (!Get_WorldItem() && pPlayer != nullptr)
	{
		CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight());

		ITEMTYPEID ItemID = {};

		if (ItemType != nullptr)
			ItemID = ItemType->Get_ItemTag();

		if (ItemID.eItemID == ITEMID::WEAPON_EPICBOW && pPlayer->Get_ItemEquipRight())
		{
			m_pAnimator->Render_Animator();
			//m_pTexture->Render_Texture();
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


}

HRESULT CEpicBow::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture[(_uint)STATE::IDLE] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EpicBowIdle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::ROMIMG] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EpicBowRoming"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::ATTACK] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EpicBowAttack"));
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

void CEpicBow::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (!(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
	// 몬스터거나 플레이어면 밀어내지않는다.
}

void CEpicBow::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CEpicBow::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

CEpicBow* CEpicBow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item)
{
	CEpicBow* pInstance = new CEpicBow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Item)))
	{
		Safe_Release(pInstance);

		MSG_BOX("TempItem Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEpicBow::Free()
{
	__super::Free();
}
