#include "stdafx.h"
#include "..\Header\Monster_Move.h"
#include "DungeonSpider.h"
#include "Player.h"
#include "SoundManager.h"
#include "Export_Function.h"
#include <random>

CMonster_Move::CMonster_Move()
{
}

CMonster_Move::CMonster_Move(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{

}

CMonster_Move::~CMonster_Move()
{
}

HRESULT CMonster_Move::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;

	m_fDistance = 30.f;
	m_fAngle = 0.f;
	m_fChase = 10.f;
	m_fSpeed = static_cast<CMonster*>(pOwner->Get_Host())->Get_BasicStat()->Get_Stat()->fSpeed;
	m_fRandomRange = 50.f;  // 이건 변수 넣는걸로 한다치고.
	m_vSavePos = _vec3(0.f, 0.f, 0.f);
	m_vRandomPos = _vec3(0.f, 0.f, 0.f);
	m_fAttackCool = 0.f;
	m_fLerpTime = 0.f;
	m_fReturnTime = 0.f;
	m_bAttackCool = false;
	m_bCheck = false;
	m_bInSightMove = false;

	return S_OK;
}

STATE CMonster_Move::Update_State(const _float& fTimeDelta)
{
	CPlayer& pPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();
	_vec3 vMonsterPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	
	_vec3 vCenterPos = _vec3(static_cast<CMonster*>(m_pOwner->Get_Host())->Get_CenterPos().x, vMonsterPos.y, static_cast<CMonster*>(m_pOwner->Get_Host())->Get_CenterPos().z);
	_vec3 vTargetCenter = _vec3(pPlayer.m_pTransform->m_vInfo[INFO_POS].x, vMonsterPos.y, pPlayer.m_pTransform->m_vInfo[INFO_POS].z);
	_float fMoveRange = static_cast<CMonster*>(m_pOwner->Get_Host())->Get_MoveRange();

	_vec3 vDirToCenter = vCenterPos - vMonsterPos;
	_float fDistanceToCenter = D3DXVec3Length(&vDirToCenter);
	_float fDistanceToPlayer = D3DXVec3Length(&(pPlayer.m_pTransform->m_vInfo[INFO_POS] - vMonsterPos));


	if (m_pOwner->Get_PrevState() == STATE::ATTACK)
	{
		m_fAttackCool += fTimeDelta;

		if (m_fAttackCool >= 2.f)
		{
			m_bAttackCool = false;
			m_fAttackCool = 0.f;
		}
	}

	if (fDistanceToPlayer < m_fChase)
	{
		_vec3 vTarget = pPlayer.m_pTransform->m_vInfo[INFO_POS] - vMonsterPos;
		D3DXVec3Normalize(&vTarget, &vTarget);

		if(m_bInSightMove)
			Lerp_Move(pPlayer.m_pTransform->m_vInfo[INFO_POS], fTimeDelta);
		else
			Get_RandomDir(m_vSavePos, vTargetCenter, fMoveRange);
		
	   
		if (!m_bAttackCool)
		{
			m_bAttackCool = true;
			return STATE::ATTACK;
		}
		//m_pOwner->Get_Transform()->Translate(vTarget * 2 * fTimeDelta);
	}
	else if (fDistanceToCenter > fMoveRange)
	{
		D3DXVec3Normalize(&vDirToCenter, &vDirToCenter);
		m_pOwner->Get_Transform()->Translate(vDirToCenter * static_cast<CMonster*>(m_pOwner->Get_Host())->Get_BasicStat()->Get_Stat()->fSpeed * fTimeDelta);

		m_fReturnTime += fTimeDelta;

		if (m_fReturnTime >= 2.5f)
		{
			//static_cast<CMonster*>(m_pOwner->Get_Host())->Set_CenterPos(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
			m_fReturnTime = 0.f;
			m_bCheck = false;
		}
	}
	else
	{
		m_fReturnTime = 0.f;

		if (m_bCheck)
			Lerp_Move(vMonsterPos, fTimeDelta);
		else
			Get_RandomDir(m_vSavePos, vCenterPos, fMoveRange);
	}

	return STATE::ROMIMG;
}


void CMonster_Move::Move_Sound()
{
	MONSTERTAG _eMonsterTag = static_cast<CMonster*>(m_pOwner->Get_Host())->Get_MonsterTag();

	switch (_eMonsterTag)
	{
	case MONSTERTAG::SPIDER:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SPIDER);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERHIT);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERIDLE);
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERMOVE);
		CSoundManager::GetInstance()->PlaySound(L"spider_walk.mp3", CHANNELID::SOUND_SPIDER, 1.f);
		break;
	case MONSTERTAG::WARRIOR:
		break;
	case MONSTERTAG::BAT:
		break;
	case MONSTERTAG::WIZARD:
		break;
	case MONSTERTAG::ALIEN:
		break;
	case MONSTERTAG::SLIME:
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SLIME);
	/*	CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERHIT);
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERIDLE);*/
		//CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_MONSTERMOVE);
		CSoundManager::GetInstance()->PlaySound(L"en_slime_idle_03.mp3", CHANNELID::SOUND_SLIME, 1.f);
		break;
	case MONSTERTAG::SKELETON:
		break;
	case MONSTERTAG::SKULLGHOST:
		break;
	case MONSTERTAG::WORM:
		break;
	}
}

void CMonster_Move::LateUpdate_State()
{
}

void CMonster_Move::Render_State()
{
	//cout << "Spider Move" << endl;
}

CMonster_Move* CMonster_Move::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMonster_Move* pState = new CMonster_Move(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMonster_Move*>(pState);

		MSG_BOX("Create Monster Move Failed");
		return nullptr;
	}

	return pState;
}

void CMonster_Move::Get_RandomDir(_vec3& vDir, const _vec3& _vCenter, const _float& fRange)
{
	_float fRandomAngle = static_cast<_float>(rand()) / RAND_MAX * 2 * D3DX_PI;

	_vec3 vRandomDir = _vec3(cosf(fRandomAngle), 0.f, sinf(fRandomAngle));

	m_vSavePos = _vCenter + vRandomDir * fRange * 0.5f;
	m_bCheck = true;
	m_bInSightMove = true;
}

void CMonster_Move::Lerp_Move(_vec3& vDir, const _float& fTimeDelta)
{
	_float fDistanceToTarget = D3DXVec3Length(&(m_vSavePos - vDir));
	_float fLerpDistance = static_cast<CMonster*>(m_pOwner->Get_Host())->Get_BasicStat()->Get_Stat()->fSpeed * fTimeDelta;

	if (fDistanceToTarget <= fLerpDistance)
	{
		m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = m_vSavePos;
		m_bCheck = false;
		m_bInSightMove = false;
	}
	else
	{
		_vec3 vLerpDir = (m_vSavePos - vDir) / fDistanceToTarget;
		m_pOwner->Get_Transform()->Translate(vLerpDir * fLerpDistance);

		m_fLerpTime += fTimeDelta;

		if (m_fLerpTime >= 2.5f)
		{
			//static_cast<CMonster*>(m_pOwner->Get_Host())->Set_CenterPos(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
			m_fLerpTime = 0.f;
			m_bCheck = false;
		}
	}
}


void CMonster_Move::Free()
{
	__super::Free();
}
