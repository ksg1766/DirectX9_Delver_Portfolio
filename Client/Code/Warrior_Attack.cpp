#include "stdafx.h"
#include "SoundManager.h"
#include "..\Header\Warrior_Attack.h"
#include "Export_Function.h"
#include "DungeonSpider.h"
#include "Player.h"

CWarror_Attack::CWarror_Attack()
{
}

CWarror_Attack::CWarror_Attack(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CWarror_Attack::~CWarror_Attack()
{
}

HRESULT CWarror_Attack::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChase = 0.f;
	m_vPrevPos = _vec3(0.f, 0.f, 0.f);
	m_bIsAttack = false;
	m_fSpeed = 10.f;
	m_fTime = 0.f;

	m_bAttackTick  = false;

	return S_OK;
}

STATE CWarror_Attack::Update_State(const _float& fTimeDelta)
{
	CPlayer& pPlayer = 
		*dynamic_cast<CPlayer*>
		(SceneManager()->GetInstance()->Get_ObjectList
		(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	

	if (!m_bIsAttack)
	{		
		m_fChase += fTimeDelta;

		if (m_fChase < 1.f)
		{
			m_pOwner->Get_Animator()->Get_Animation()->Set_Frame(1.f);
			m_pOwner->Get_Animator()->Get_Animation()->Set_Loop(false);
		}
		else
		{
			dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
			m_pOwner->Get_Animator()->Get_Animation()->Set_Loop(true);
			m_vPrevPos = pPlayer.m_pTransform->m_vInfo[INFO_POS];
			m_bIsAttack = true;
			m_fChase = 0.f;
			Attack_Sound();
		}
	}

	if (m_bIsAttack)
	{
		_vec3 vDir = m_vPrevPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&vDir, &vDir);

		//m_pOwner->Get_Transform()->m_vInfo[INFO_POS] =
		//	m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + vDir * 10 * fTimeDelta;

		m_pOwner->Get_Transform()->Translate(_vec3(vDir.x, 0.f, vDir.z) * 10 * fTimeDelta);
		
		_float fDistance = D3DXVec3Length(&(pPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pOwner->Get_Transform()->m_vInfo[INFO_POS]));

		m_fTime += fTimeDelta;

		if (m_fTime >= 2.f)
		{
			m_fTime = 0.f;
			return STATE::ROMIMG;
		}
	}


	if (D3DXVec3Length(&(m_vPrevPos  - m_pOwner->Get_Transform()->m_vInfo[INFO_POS])) < 1.f
		&& m_pOwner->Get_Animator()->Get_Animation()->Get_Frame() > 4.8f)
	{
		m_bIsAttack = false;
		m_fTime = 0.f;
		return STATE::ROMIMG;
	}
}

void CWarror_Attack::LateUpdate_State()
{
}

void CWarror_Attack::Render_State()
{
	//cout << "Spider Jump" << endl;
}

void CWarror_Attack::Attack_Sound()
{
	MONSTERTAG _eMonsterTag = dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_MonsterTag();


	switch (_eMonsterTag)
	{
	case MONSTERTAG::WARRIOR:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WARRIOR);
		CSoundManager::GetInstance()->PlaySound(L"en_melee_2_attack_02.mp3", CHANNELID::SOUND_WARRIOR, 1.f);
		break;
	case MONSTERTAG::SLIME:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SLIME);
		CSoundManager::GetInstance()->PlaySound(L"en_slime_attack_01.mp3", CHANNELID::SOUND_SLIME, 1.f);
		break;
	case MONSTERTAG::SKELETON:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SKELETON);
		CSoundManager::GetInstance()->PlaySound(L"en_skel_attack_01.mp3", CHANNELID::SOUND_SKELETON, 1.f);
		break;
	}
}


CWarror_Attack* CWarror_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CWarror_Attack* pState = new CWarror_Attack(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CWarror_Attack*>(pState);

		MSG_BOX("Create Jump Failed");
		return nullptr;
	}

	return pState;
}

void CWarror_Attack::Free()
{
	__super::Free();
}
