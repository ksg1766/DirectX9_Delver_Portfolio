#include "stdafx.h"
#include "SoundManager.h"
#include "..\Header\Monster_Jump.h"
#include "Export_Function.h"
#include "DungeonSpider.h"
#include "Worm.h"
#include "Player.h"

CMonster_Jump::CMonster_Jump()
{
}

CMonster_Jump::CMonster_Jump(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev), m_bIsJumping(true)
{
}

CMonster_Jump::~CMonster_Jump()
{
}

HRESULT CMonster_Jump::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fJumpVelocity = 15.f;
	m_bJumCoolDown = false;
	m_bIsJumping = false;
	m_bStartJumpPos = true;
	m_fJumpCoolDuration = 2.f;
	m_fJumpCoolTimer = 0.f;
	m_vLastPos = _vec3(0.f, 0.f, 0.f);
	m_vSavePos = _vec3(0.f, 0.f, 0.f);
	return S_OK;
}

STATE CMonster_Jump::Update_State(const _float& fTimeDelta)
{
	return Jump(fTimeDelta);
}

void CMonster_Jump::LateUpdate_State()
{
}

void CMonster_Jump::Render_State()
{
	//cout << "Spider Jump" << endl;
}

STATE CMonster_Jump::Jump(const _float& fTimeDelta)
{
	CPlayer& rPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());


	if (!m_bIsJumping)
	{
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTER);

		m_fChase += fTimeDelta;

		if (m_fChase < 0.1f)
		{
			m_pOwner->Get_Animator()->Get_Animation()->Set_Frame(1.f);
			m_pOwner->Get_Animator()->Get_Animation()->Set_Loop(false);
		}
		else
		{
			dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
			dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_JumpVelocity(15.f);
			m_pOwner->Get_Animator()->Get_Animation()->Set_Loop(true);
			m_vLastPos = rPlayer.m_pTransform->m_vInfo[INFO_POS];
			m_bIsJumping = true;
			m_fChase = 0.f;
		}
	}


	if (m_bIsJumping)
	{
		//_vec3 vMonsterPos = _vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x, 0.f, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z);
		_vec3 vDir = m_vLastPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&vDir, &vDir);

		_vec3 vTest = _vec3(vDir.x, 0.f, vDir.z);


		m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y += dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_JumpVelocity() * fTimeDelta;
		m_pOwner->Get_Transform()->m_vInfo[INFO_POS] += vDir * 10 * fTimeDelta;


		if(dynamic_cast<CMonster*>(m_pOwner->Get_Host())->IsJump())
		dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Add_JumpVelocity(-0.5f * fTimeDelta * fTimeDelta * 3000.f);
	

		_float fDistance = D3DXVec3Length(&(rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pOwner->Get_Transform()->m_vInfo[INFO_POS]));

		if (fDistance < 15.f)
			CSoundManager::GetInstance()->PlaySound(L"en_spider_attack_01.mp3", CHANNELID::SOUND_MONSTER, 1.f);
	}

	
	if (!dynamic_cast<CMonster*>(m_pOwner->Get_Host())->IsJump())
	{
		m_bIsJumping = false;
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTER);
		CSoundManager::GetInstance()->PlaySound(L"spider_walk.mp3", CHANNELID::SOUND_MONSTER, 1.f);
		return STATE::ROMIMG;
	}



	//m_pOwner->Get_Animator()->Get_Animation()->Set_Frame(0.f);
	return STATE::ATTACK;

}

CMonster_Jump* CMonster_Jump::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMonster_Jump* pState = new CMonster_Jump(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMonster_Jump*>(pState);

		MSG_BOX("Create Jump Failed");
		return nullptr;
	}

	return pState;
}

void CMonster_Jump::Free()
{
	__super::Free();
}
