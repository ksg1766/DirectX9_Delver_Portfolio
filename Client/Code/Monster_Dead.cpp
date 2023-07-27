#include "stdafx.h"
#include "SoundManager.h"
#include "..\Header\Monster_Dead.h"
#include "Export_Function.h"
#include "Player.h"

CMonster_Dead::CMonster_Dead()
{
}

CMonster_Dead::CMonster_Dead(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CMonster_Dead::~CMonster_Dead()
{
}

HRESULT CMonster_Dead::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fFlyDown = 7.f;

	return S_OK;
}

STATE CMonster_Dead::Update_State(const _float& fTimeDelta)
{

	if (!m_bDead)
	{
		Monster_Dead();
		m_bDead = true;
	}

	if (dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_MonsterTag() == MONSTERTAG::BAT)
	{
		m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y += m_fFlyDown * fTimeDelta;

		m_fFlyDown -= 0.5f * fTimeDelta * fTimeDelta * 3000.f;

		if (m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y < 1.f)
			m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y = 0.5f;
	}

	return STATE::DEAD;
}

void CMonster_Dead::LateUpdate_State()
{
}

void CMonster_Dead::Render_State()
{
}

void CMonster_Dead::Monster_Dead()
{
	MONSTERTAG _eMonsterTag = dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_MonsterTag();

	switch (_eMonsterTag)
	{
	case MONSTERTAG::SPIDER:
	/*	CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERHIT);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERATTACK);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERIDLE);*/
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SPIDER);
		CSoundManager::GetInstance()->PlaySound(L"en_spider_die_01.mp3", CHANNELID::SOUND_SPIDER, 1.f);
		break;
	case MONSTERTAG::WARRIOR:
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERHIT);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERATTACK);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERIDLE);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WARRIOR);
		CSoundManager::GetInstance()->PlaySound(L"en_melee_2_die_02.mp3", CHANNELID::SOUND_WARRIOR, 1.f);
		break;
	case MONSTERTAG::BAT:
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERHIT);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERATTACK);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERIDLE);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_BAT);
		CSoundManager::GetInstance()->PlaySound(L"en_bat_death_01.mp3", CHANNELID::SOUND_BAT, 1.f);
		break;
	case MONSTERTAG::WIZARD:
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERHIT);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERATTACK);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERIDLE);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WIZARD);
		CSoundManager::GetInstance()->PlaySound(L"en_mage_die_01.mp3", CHANNELID::SOUND_WIZARD, 1.f);
		break;
	case MONSTERTAG::ALIEN:
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERHIT);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERATTACK);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERIDLE);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_ALIEN);
		CSoundManager::GetInstance()->PlaySound(L"en_beholder_die_01.mp3", CHANNELID::SOUND_ALIEN, 1.f);
		break;
	case MONSTERTAG::SLIME:
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERHIT);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERATTACK);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERIDLE);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SLIME);
		CSoundManager::GetInstance()->PlaySound(L"en_slime_die_03.mp3", CHANNELID::SOUND_SLIME, 1.f);
		break;
	case MONSTERTAG::SKELETON:
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERHIT);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERATTACK);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERIDLE);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SKELETON);
		CSoundManager::GetInstance()->PlaySound(L"en_slime_die_03.mp3", CHANNELID::SOUND_SKELETON, 1.f);
		break;
	case MONSTERTAG::SKULLGHOST:
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERHIT);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERATTACK);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERIDLE);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SKULL);
		CSoundManager::GetInstance()->PlaySound(L"en_skull_die_01", CHANNELID::SOUND_SKULL, 1.f);
		break;
	case MONSTERTAG::WORM:
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERHIT);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERATTACK);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERIDLE);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WORM);
		CSoundManager::GetInstance()->PlaySound(L"en_worm_die_01.mp3", CHANNELID::SOUND_WORM, 1.f);
		break;
	}
}

CMonster_Dead* CMonster_Dead::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMonster_Dead* pState = new CMonster_Dead(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMonster_Dead*>(pState);

		MSG_BOX("Create Monster Dead Failed");
		return nullptr;
	}

	return pState;
}

void CMonster_Dead::Free()
{
	__super::Free();
}
