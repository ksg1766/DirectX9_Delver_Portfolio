#include "..\Header\Test_Move.h"
#include "Export_Function.h"
#include "Player.h"

CTest_Move::CTest_Move()
{
}

CTest_Move::CTest_Move(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CTest_Move::~CTest_Move()
{
}

HRESULT CTest_Move::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;


	m_bJumCoolDown = false;
	m_fJumpCoolTimer = 0.f;

	m_vRandomPos = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

STATE CTest_Move::Update_State(const _float& fTimeDelta)
{
	if (m_pOwner->Get_PrevState() == STATE::ATTACK || m_pOwner->Get_PrevState() == STATE::HIT)
		m_bJumCoolDown = true;

	CPlayer& rPlayer = *dynamic_cast<CPlayer*>(SceneManager()->
		GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (m_bJumCoolDown) // 이 전의 State가 공격이나 히트였다면 쿨타임이 돈다.
	{
		m_fJumpCoolTimer += fTimeDelta; // 쿨타임 시간이 어느정도 지났다면

		if (m_fJumpCoolTimer >= m_fJumpCoolDuration) // 2초보다 더 크다면 다시 공격상태로 돌입.
		{
			m_bJumCoolDown = false; // 쿨다운 해제
			m_fJumpCoolTimer = 0.f; // 쿨타임 시간 리셋 
			dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
			return STATE::ATTACK;
		}
		else // 작다면 플레이어 기준으로 랜덤한 위치로 이동.
		{
			_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
			// 현재 본인위치.
			_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - vCurrentPos;
			// 현재 플레이어와의 방향.
			
			_float fAngle = (_float)rand() / 2.f * D3DX_PI;
			m_vSavePos.x = (10 * cosf(fAngle)) / 100.f;
			m_vSavePos.z = (10 * -sinf(fAngle)) / 100.f;
			
			
			D3DXVec3Normalize(&m_vSavePos, &m_vSavePos); // 정규화 시킨다. 근데 플레이어의 방향에서 랜덤하게.
			
			m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + m_vSavePos * 5.f * fTimeDelta;
		}
	}

	// 플레이어 정보 불러옴.

	_vec3 vPlayerPos = rPlayer.m_pTransform->m_vInfo[INFO_POS]; // 플레이어 위치
	_vec3 vDistance = vPlayerPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS]; // 플레이어와의 방향

	_float fLength = D3DXVec3Length(&vDistance); // 구한 방향벡터의 스칼라를 구함.
	_float fSight = 10.f; // 일단 상수 값으로 

	if (fLength < fSight) // 플레이어와 거리가 시야보다 작을경우 공격하도록.
	{
		return STATE::ATTACK;
	}

	_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	// 현재 본인위치.
	_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - vCurrentPos;
	// 현재 플레이어와의 방향.

	_float fAngle = (_float)rand() / 2.f * D3DX_PI;
	_float X = m_vSavePos.x + (10 * cosf(fAngle)) / 100.f;
	_float Z = m_vSavePos.z + (10 * -sinf(fAngle)) / 100.f;

	m_vSavePos = _vec3(X, 0, Z);

	D3DXVec3Normalize(&m_vSavePos, &m_vSavePos); // 정규화 시킨다. 근데 플레이어의 방향에서 랜덤하게.

	m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + m_vSavePos * 5.f * fTimeDelta;

	return STATE::ROMIMG;
}

void CTest_Move::LateUpdate_State()
{
}

void CTest_Move::Render_State()
{
}

CTest_Move* CTest_Move::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CTest_Move* pState = new CTest_Move(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CTest_Move*>(pState);

		MSG_BOX("Create Test Move Failed");
		return nullptr;
	}

	return pState;
}

void CTest_Move::Free()
{
	__super::Free();
}
