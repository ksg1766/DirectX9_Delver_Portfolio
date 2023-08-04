#include "..\Header\Monster_Fly.h"
#include "Export_Function.h"
#include "Player.h"

CMonster_Fly::CMonster_Fly()
{
}

CMonster_Fly::CMonster_Fly(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev), m_vSavePos(_vec3(0.f,0.f,0.f))
{
}

CMonster_Fly::~CMonster_Fly()
{
}

HRESULT CMonster_Fly::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_bAttackCoolDown = false;
	m_bFirstCool = true;
	m_fAttackDuration = 3.f;
	m_fChase = 0.f;
	m_fReturnTime = 0.f;
	m_fLerpTime = 0.f;
	m_bInSightMove = false;

	return S_OK;
}

STATE CMonster_Fly::Update_State(const _float& fTimeDelta)
{
	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();
	_vec3 vMonsterPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

	_vec3 vCenterPos = static_cast<CMonster*>(m_pOwner->Get_Host())->Get_CenterPos();
	_vec3 vTargetCenter = _vec3(rPlayer.m_pTransform->m_vInfo[INFO_POS].x, vMonsterPos.y, rPlayer.m_pTransform->m_vInfo[INFO_POS].z);
	_float fMoveRange = static_cast<CMonster*>(m_pOwner->Get_Host())->Get_MoveRange();

	_vec3 vDirToCenter = vCenterPos - vMonsterPos;
	_float fDistanceToCenter = D3DXVec3Length(&vDirToCenter);
	_float fDistanceToPlayer = D3DXVec3Length(&(rPlayer.m_pTransform->m_vInfo[INFO_POS] - vMonsterPos));

	if (m_bAttackCoolDown)
	{
		m_fChase += fTimeDelta;

		if (m_fChase >= m_fAttackDuration)
		{
			m_bAttackCoolDown = false;
			m_fChase = 0.f;
			static_cast<CMonster*>(m_pOwner->Get_Host())->Set_State(STATE::ATTACK);
			static_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
		}
	}

	if (fDistanceToPlayer < 10.f)
	{
		_vec3 vTarget = rPlayer.m_pTransform->m_vInfo[INFO_POS] - vMonsterPos;
		D3DXVec3Normalize(&vTarget, &vTarget);

		if (m_bInSightMove)
			Lerp_Move(rPlayer.m_pTransform->m_vInfo[INFO_POS], fTimeDelta);
		else
			Get_RandomDir(m_vSavePos, vTargetCenter, fMoveRange);

		if (!m_bAttackCoolDown)
		{
			m_bAttackCoolDown = true;
			return STATE::ATTACK;
		}
	}
	else if (fDistanceToCenter > fMoveRange)
	{
		D3DXVec3Normalize(&vDirToCenter, &vDirToCenter);
		m_pOwner->Get_Transform()->Translate(vDirToCenter * static_cast<CMonster*>(m_pOwner->Get_Host())->Get_BasicStat()->Get_Stat()->fSpeed * fTimeDelta);

		m_fReturnTime += fTimeDelta;

		if (m_fReturnTime >= 2.5f)
		{
			static_cast<CMonster*>(m_pOwner->Get_Host())->Set_CenterPos(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
			m_fReturnTime = 0.f;
			m_bCheck = false;
		}
	}
	else
	{
		if (m_bCheck)
			Lerp_Move(vMonsterPos, fTimeDelta);
		else
			Get_RandomDir(m_vSavePos, vCenterPos, fMoveRange);
	}


	return STATE::ROMIMG;
}

void CMonster_Fly::LateUpdate_State()
{
}

void CMonster_Fly::Render_State()
{
	//cout << "Fly" << endl;
}

void CMonster_Fly::Get_RandomDir(_vec3& vDir, const _vec3& _vCenter, const _float& fRange)
{
	_float fRandomAngle = static_cast<_float>(rand()) / RAND_MAX * 2 * D3DX_PI;

	_vec3 vRandomDir = _vec3(cosf(fRandomAngle), -sinf(fRandomAngle), cosf(fRandomAngle));

	m_vSavePos = _vCenter + vRandomDir * fRange * 0.5f;
	m_bCheck = true;
	m_bInSightMove = true;
}

void CMonster_Fly::Lerp_Move(_vec3& vDir, const _float& fTimeDelta)
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

		if (m_fLerpTime >= 2.0f)
		{
			static_cast<CMonster*>(m_pOwner->Get_Host())->Set_CenterPos(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
			m_fLerpTime = 0.f;
			m_bCheck = false;
		}
	}
}

CMonster_Fly* CMonster_Fly::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMonster_Fly* pState = new CMonster_Fly(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMonster_Fly*>(pState);

		MSG_BOX("Create Fly Failed");
		return nullptr;
	}

	return pState;
}

void CMonster_Fly::Free()
{
	__super::Free();
}
