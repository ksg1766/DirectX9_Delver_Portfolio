#include "stdafx.h"
#include "SoundManager.h"
#include "..\Header\Monster_Hit.h"
#include "Export_Function.h"
#include "Player.h"

CMonster_Hit::CMonster_Hit()
{
}

CMonster_Hit::CMonster_Hit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CMonster_Hit::~CMonster_Hit()
{
}

HRESULT CMonster_Hit::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChase = 0.f;


	m_bCanHitState = true;
	m_fHitCoolDown = 0.f;
	m_iHitCount = 0;
	return S_OK;
}

STATE CMonster_Hit::Update_State(const _float& fTimeDelta)
{
	
	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()
		->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	CItem* ItemType = dynamic_cast<CItem*>(pPlayer.Get_CurrentEquipRight());
	ITEMTYPEID ItemID = {};

	m_fChase += fTimeDelta;

	if (m_bCanHitState)
	{
		Hit_Sound();
		m_bCanHitState = false;
	}

	if (m_fChase <= 1.0f)
	{
		
		if(ItemID.eItemID == ITEMID::WEAPON_SWORD)
		m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS] += 
			pPlayer.m_pTransform->m_vInfo[INFO_LOOK] * 5 * fTimeDelta;

		if (ItemID.eItemID == ITEMID::WEAPON_BOW)
			m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS] +=
			pPlayer.m_pTransform->m_vInfo[INFO_LOOK] * 3 * fTimeDelta;

	
	}
	else
	{
		m_fChase = 0.f;
		m_bCanHitState = true;
		return STATE::ROMIMG;
	}




}

void CMonster_Hit::LateUpdate_State()
{
}

void CMonster_Hit::Render_State()
{
}

void CMonster_Hit::Hit_Sound()
{
	MONSTERTAG _eMonsterTag = dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_MonsterTag();


	switch (_eMonsterTag)
	{
	case MONSTERTAG::SPIDER:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTER);
		CSoundManager::GetInstance()->PlaySound(L"en_spider_hurt_01.mp3", CHANNELID::SOUND_MONSTER, 1.f);
		break;
	case MONSTERTAG::WARRIOR:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTER);
		CSoundManager::GetInstance()->PlaySound(L"en_melee_2_hurt_02.mp3", CHANNELID::SOUND_MONSTER, 1.f);
		break;
	case MONSTERTAG::BAT:
		break;
	case MONSTERTAG::WIZARD:
		break;
	case MONSTERTAG::ALIEN:
		break;
	case MONSTERTAG::SLIME:
		break;
	case MONSTERTAG::SKELETON:
		break;
	case MONSTERTAG::SKULLGHOST:
		break;
	case MONSTERTAG::WORM:
		break;
	}
}

CMonster_Hit* CMonster_Hit::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMonster_Hit* pState = new CMonster_Hit(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMonster_Hit*>(pState);

		MSG_BOX("Create Monster Hit Failed");
		return nullptr;
	}

	return pState;
}

void CMonster_Hit::Free()
{
	__super::Free();
}
