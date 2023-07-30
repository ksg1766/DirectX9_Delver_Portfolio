
#include "stdafx.h"

#include "SoundManager.h"
#include "DynamicCamera.h"
#include "..\Header\TempItem.h"
#include "Export_Function.h"
#include "Player.h"
#include "EffectSquare.h"
#include "EffectDamage.h"
#include "CameraManager.h"
#include "FlyingCamera.h"

static _int g_iCount = 0;

CTempItem::CTempItem(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CItem(pGraphicDev)
{
}

CTempItem::CTempItem(const CTempItem& rhs)
	: Engine::CItem(rhs)
{
}

CTempItem::~CTempItem()
{
	Free();
}

HRESULT CTempItem::Ready_Object(_bool _Item)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_bWorldItem = _Item;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!Get_WorldItem())
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
		m_pCollider->
			InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

		m_iAttackTick = 10;
		m_iMoveTick = 10;
		// 타입 및 아이디 지정
		m_ItemID.eItemType = ITEMTYPE_WEAPONITEM;
		m_ItemID.eItemID = WEAPON_SWORD;
		m_ItemID.iCount = 1;

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTransform->m_pParent->Get_Host());
		//m_pTransform->Translate(pPlayer->m_pTransform->m_vInfo[INFO_POS] + *dynamic_cast<CPlayer*>(pPlayer)->Get_Offset());

#pragma region ksg

		CTransform* pPlayerTransform = pPlayer->m_pTransform;

		_vec3 vOffSet = 0.7f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.4f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
		m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);

#pragma endregion ksg
	}
	else
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
		m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

		m_pTransform->Translate(_vec3(0.0f, 2.f, 0.0f));

		m_iAttackTick = 10;

		// 타입 및 아이디 지정
		m_ItemID.eItemType = ITEMTYPE_WEAPONITEM;
		m_ItemID.eItemID = WEAPON_SWORD;
		m_ItemID.iCount = 1;
	}

#pragma region SwoardStat
	m_pBasicStat->Get_Stat()->iDamageMax = 2.f;
	m_pBasicStat->Get_Stat()->iDamageMin = 1.f;
#pragma endregion

	return S_OK;
}

_int CTempItem::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());
	CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight());
	ITEMTYPEID ItemID = {};

	if (ItemType != nullptr)
		ItemID = ItemType->Get_ItemTag();

	if (ItemID.eItemID != ITEMID::WEAPON_SWORD || !pPlayer->Get_ItemEquipRight())
		return iExit;

	if (!Get_WorldItem())
	{
#pragma region ksg
		if (pPlayer->Get_Attack() && pPlayer != nullptr)
		{
			m_iMoveTick = 0;
			if (m_iAttackTick > 0)
			{
				m_pTransform->Translate(m_pTransform->m_vInfo[INFO_LOOK] * 0.2f);
				m_pTransform->Rotate(ROT_X, -0.1f);
				m_pTransform->Rotate(ROT_Y, -0.05f);
				m_pTransform->Rotate(ROT_Z, 0.02f);
			}
			else
			{
				m_pTransform->Translate(m_pTransform->m_vInfo[INFO_LOOK] * -0.2f);
				m_pTransform->Rotate(ROT_Z, -0.02f);
				m_pTransform->Rotate(ROT_Y, 0.05f);
				m_pTransform->Rotate(ROT_X, 0.1f);
			}
			--m_iAttackTick;

			if (-9 == m_iAttackTick)
			{
				m_iAttackTick = 10;
				pPlayer->Set_Attack(false);

				CTransform* pPlayerTransform = pPlayer->m_pTransform;

				_vec3 vOffSet = 0.7f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.4f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
				m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);

				_vec3 vLocalScale = m_pTransform->LocalScale();

				m_pTransform->Copy_RUL(pPlayerTransform->m_vInfo);
				for (_int i = 0; i < INFO_POS; ++i)
					m_pTransform->m_vInfo[i] *= *(((_float*)&vLocalScale) + i);
			}
		}
		else if (pPlayer->Get_StateMachine()->Get_State() == STATE::ROMIMG && !pPlayer->IsJump())
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
			CTransform* pPlayerTransform = pPlayer->m_pTransform;

			_vec3 vOffSet = 0.7f * pPlayerTransform->m_vInfo[INFO_RIGHT] + 1.4f * pPlayerTransform->m_vInfo[INFO_LOOK] - 0.4f * pPlayerTransform->m_vInfo[INFO_UP];
			m_pTransform->m_vInfo[INFO_POS] = (pPlayerTransform->m_vInfo[INFO_POS] + vOffSet);
			m_iMoveTick = 0;
		}
#pragma endregion ksg

	}

	return iExit;
}

void CTempItem::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

}


void CTempItem::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

	if (!Get_WorldItem() && pPlayer != nullptr)
	{
		CItem* ItemType = dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight());

		ITEMTYPEID ItemID = {};

		if (ItemType != nullptr)
			ItemID = ItemType->Get_ItemTag();

		if (ItemID.eItemID == ITEMID::WEAPON_SWORD && pPlayer->Get_ItemEquipRight())
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

HRESULT CTempItem::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Sword"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	if (!Get_WorldItem())
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

		m_pTransform->Set_Parent(pPlayer->m_pTransform);
		m_pTransform->Copy_RUL(pPlayer->m_pTransform->m_vInfo);
	}
	else
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

void CTempItem::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	/*if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)&& !(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BOSS))
	{
		__super::OnCollisionEnter(_pOther);
	}*/

	// 몬스터거나 플레이어면 밀어내지않는다.

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());
	// 플레이어의 정보를 레퍼런스로 얻어옴.

	if ((_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)||(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BOSS))
		// 무기 콜리전에 들어온 타입이 몬스터이면서, 플레이어의 스테이트가 공격이라면
	{
		if ((!pPlayer.Get_AttackTick() &&
			dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_StateMachine()->Get_State() != STATE::DEAD))
			// 공격 하지 않은 상태라면.
		{
			pPlayer.Set_AttackTick(true);
			dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_StateMachine()->Set_State(STATE::HIT);
			

			//////////////////////////////////////////////////////////////////////////////// 이펙트 
			_matrix      matMonsterWorld  = _pOther->Get_Host()->m_pTransform->WorldMatrix();
			_vec3        vecMonsterPos    = _vec3(matMonsterWorld._41, matMonsterWorld._42 + .5f, matMonsterWorld._43);
			CGameObject* pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_NONE);
			dynamic_cast<CEffectSquare*>(pGameObject)->Set_MonsterEffectColor(dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_MonsterTag());
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

			pGameObject = CEffectDamage::Create(m_pGraphicDev);
			pGameObject->m_pTransform->Translate(_vec3(vecMonsterPos.x, vecMonsterPos.y - 0.5f, vecMonsterPos.z + .5f));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			//////////////////////////////////////////////////////////////////////////////// 이펙트 

			//cout << "데미지" << endl;
		}
	}
	else if (!pPlayer.Get_AttackTick() && _pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BLOCK)
	{
		pPlayer.Set_AttackTick(true);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_BREAK);
		CSoundManager::GetInstance()->PlaySound(L"clang_02.mp3", CHANNELID::SOUND_BREAK, 1.f);
	}
}

void CTempItem::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	/*if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) && !(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
	{
		__super::OnCollisionStay(_pOther);
	}*/

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());
	// 플레이어의 정보를 레퍼런스로 얻어옴.

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER && !pPlayer.Get_AttackTick() &&
		dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_StateMachine()->Get_State() != STATE::DEAD)
	{
		++m_iHitCount;

		if (m_iHitCount == 2)
		{
			m_iHitCount = 0;
			dynamic_cast<CMonster*>(_pOther->Get_Host())->Set_KnockBack(true);
		}
		pPlayer.IsAttack(dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_BasicStat());

		//////////////////////////////////////////////////////////////////////////////// 이펙트 
		_matrix MonsterWorld     = _pOther->Get_Host()->m_pTransform->WorldMatrix();
		_vec3   vecMonsterPos    = _vec3(MonsterWorld._41, MonsterWorld._42 + .5f, MonsterWorld._43);
		CGameObject* pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_NONE);
		dynamic_cast<CEffectSquare*>(pGameObject)->Set_MonsterEffectColor(dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_MonsterTag());
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		//////////////////////////////////////////////////////////////////////////////// 이펙트 
	}
	else if (!pPlayer.Get_AttackTick() && _pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BLOCK)
	{
		pPlayer.Set_AttackTick(true);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_BREAK);
		CSoundManager::GetInstance()->PlaySound(L"clang_02.mp3", CHANNELID::SOUND_BREAK, 1.f);

		dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Shake_Camera();
	}
}

void CTempItem::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	/*if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
	{
		__super::OnCollisionExit(_pOther);
	}*/

}

CTempItem* CTempItem::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item)
{
	CTempItem* pInstance = new CTempItem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Item)))
	{
		Safe_Release(pInstance);

		MSG_BOX("TempItem Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTempItem::Free()
{
	__super::Free();
}