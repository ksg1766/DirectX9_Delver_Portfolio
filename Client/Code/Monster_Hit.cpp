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
	
	CPlayer& pPlayer = *static_cast<CPlayer*>(SceneManager()->GetInstance()
		->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	CItem* ItemType = static_cast<CItem*>(pPlayer.Get_CurrentEquipRight());
	ITEMTYPEID ItemID = {};

	m_fChase += fTimeDelta;

	if (m_bCanHitState)
	{
		if (static_cast<CMonster*>(m_pOwner->Get_Host())->Get_BasicStat()->Get_Stat()->fHP > 0)
		{
			m_bCanHitState = false;
			Hit_Sound();
		}
		else
			return STATE::DEAD;
	}

	if (m_fChase <= 1.2f)
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
		m_bCanHitState = true;
		m_fChase = 0.f;
		return STATE::ATTACK;
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
	MONSTERTAG _eMonsterTag = static_cast<CMonster*>(m_pOwner->Get_Host())->Get_MonsterTag();


	switch (_eMonsterTag)
	{
	case MONSTERTAG::SPIDER:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SPIDER);
		CSoundManager::GetInstance()->PlaySound(L"en_spider_hurt_01.mp3", CHANNELID::SOUND_SPIDER, 1.f);
		break;
	case MONSTERTAG::WARRIOR:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WARRIOR);
		CSoundManager::GetInstance()->PlaySound(L"en_melee_2_hurt_02.mp3", CHANNELID::SOUND_WARRIOR, 1.f);
		break;
	case MONSTERTAG::BAT:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_BAT);
		CSoundManager::GetInstance()->PlaySound(L"en_bat_hurt_01.mp3", CHANNELID::SOUND_BAT, 1.f);
		break;
	case MONSTERTAG::WIZARD:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WIZARD);
		CSoundManager::GetInstance()->PlaySound(L"en_mage_hurt_03.mp3", CHANNELID::SOUND_WIZARD, 1.f);
		break;
	case MONSTERTAG::ALIEN:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_ALIEN);
		CSoundManager::GetInstance()->PlaySound(L"en_beholder_hurt_01.mp3", CHANNELID::SOUND_ALIEN, 1.f);
		break;
	case MONSTERTAG::SLIME:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SLIME);
		CSoundManager::GetInstance()->PlaySound(L"en_slime_hurt_01.mp3", CHANNELID::SOUND_SLIME, 1.f);
		break;
	case MONSTERTAG::SKELETON:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SKELETON);
		CSoundManager::GetInstance()->PlaySound(L"en_skel_hurt_02.mp3", CHANNELID::SOUND_SKELETON, 1.f);
		break;
	case MONSTERTAG::SKULLGHOST:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SKULL);
		CSoundManager::GetInstance()->PlaySound(L"en_skull_hurt_03.mp3", CHANNELID::SOUND_SKULL, 1.f);
		break;
	case MONSTERTAG::WORM:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WORM);
		CSoundManager::GetInstance()->PlaySound(L"en_worm_hurt_02.mp3", CHANNELID::SOUND_WORM, 1.f);
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
