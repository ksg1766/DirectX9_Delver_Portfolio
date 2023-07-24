#include "..\Header\Monster_Move.h"
#include "Export_Function.h"
#include "DungeonSpider.h"
#include "Player.h"

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
	m_fSpeed = dynamic_cast<CMonster*>(pOwner->Get_Host())->Get_BasicStat()->Get_Stat()->fSpeed;
	m_fRandomRange = 50.f;  // 이건 변수 넣는걸로 한다치고.
	m_vSavePos = _vec3(0.f, 0.f, 0.f);
	m_vRandomPos = _vec3(0.f, 0.f, 0.f);
	m_fAttackCool = 0.f;
	m_bAttackCool = false;
	m_bCheck = false;

	return S_OK;
}

STATE CMonster_Move::Update_State(const _float& fTimeDelta)
{
	CPlayer& pPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	if (m_pOwner->Get_PrevState() == STATE::ATTACK)
	{
		m_fAttackCool += fTimeDelta;
		if (m_fAttackCool > 2.f)
			m_bAttackCool = false;
	}

	_vec3 fDistance = m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS] - pPlayer.m_pTransform->m_vInfo[INFO_POS];
	_float fLength = D3DXVec3Length(&fDistance);

	if (fLength < m_fChase)
	{
		_vec3 fChaseDir = pPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&fChaseDir, &fChaseDir);

		
		if (dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_MonsterTag() == MONSTERTAG::SPIDER ||
			dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_MonsterTag() == MONSTERTAG::WORM)
		{
			if (!Get_AttackCool())
			{
				m_fAttackCool = 0.f;
				m_bAttackCool = true;
				dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
				return STATE::ATTACK;
			}
			else
			{
				Move_RandomPos(fTimeDelta);
			}
		}
		else
		{
			//m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS] += fChaseDir * m_fSpeed * fTimeDelta;

			if (!Get_AttackCool())
			{
				m_fAttackCool = 0.f;
				m_bAttackCool = true;
				dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
				return STATE::ATTACK;
			}
			else
			{
				Move_RandomPos(fTimeDelta);
			}
		}
	}
	else
	{
		_vec3 vReturn = dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_CenterPos() - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		_float fReturnDistance = D3DXVec3Length(&vReturn);
		D3DXVec3Normalize(&vReturn, &vReturn);

		if (fReturnDistance >= 30.f && !m_bCheck)
		{
			m_pOwner->Get_Transform()->m_vInfo[INFO_POS] += vReturn * m_fSpeed * fTimeDelta; // Distance가 큰 동안 무조건 Center 쪽으로 보냄. 근데 30이 됐을 땐 bool값을 TRUE로 바꿈
			//m_bCheck = true; // 이러면 30 영역 안에 들어왔다는 것임. 여기서부터 랜덤지역을 배회시킴. 근데 센터에서 50 ~ 100 정도 벗어나면 다시 돌아가게 만든다.
		}
		else
			m_bCheck = true;

		if (m_bCheck)
		{
			if (fReturnDistance > 50.f)
				m_bCheck = false;
			else
			{
				Move_RandomPos(fTimeDelta);
			}
		}
	}


	return STATE::ROMIMG;
}





//STATE CMonster_Move::Update_State(const _float& fTimeDelta)
//{
//
//
//	if (m_pOwner->Get_PrevState() == STATE::ATTACK || m_pOwner->Get_PrevState() == STATE::HIT)
//		m_bJumCoolDown = true;
//
//	CComponent* pPlayerTransform =
//		SceneManager()->GetInstance()->
//		Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()
//		->Get_Component(COMPONENTTAG::TRANSFORM, COMPONENTID::ID_DYNAMIC);
//
//	_vec3 vPlayerPos = static_cast<CTransform*>(pPlayerTransform)->m_vInfo[INFO_POS];
//
//	_vec3 vDistance = vPlayerPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
//	_float fDistanceLength = D3DXVec3Length(&vDistance);
//	_float fSight = pow(5, 2);
//
//
//	//if (fDistanceLength >= fSight)
//	//{
//	//	if (Reached_Pos())
//	//		Set_NewPos();
//	//	else
//	//		Move_RandomPos(fTimeDelta);
//
//	//	
//	//	return STATE::ROMIMG;
//	//}
//	//
//	//if (m_bFirstCool)
//	//{
//	//	m_bFirstCool = false;
//	//	return STATE::ATTACK;
//	//}
//
//	if (fDistanceLength <= 10.f)
//	{
//
//		if (m_bJumCoolDown)
//		{
//			m_fJumpCoolTimer += fTimeDelta;
//
//			if (m_fJumpCoolTimer >= m_fJumpCoolDuration)
//			{
//				m_bJumCoolDown = false;
//				m_fJumpCoolTimer = 0.f;
//				dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
//				return STATE::ATTACK;
//			}
//			else
//			{
//				Move_RandomPos(fTimeDelta);
//			}
//		}
//
//		return STATE::ATTACK;
//	}
//	
//	if (Reached_Pos())
//	{
//		Set_NewPos(fTimeDelta);
//		return STATE::ROMIMG;
//	}
//	else
//	{
//		Move_RandomPos(fTimeDelta);
//		return STATE::ROMIMG;
//	}
//
//	
//	m_pOwner->Set_State(STATE::ROMIMG);
//
//}
//
//void CMonster_Move::Set_NewPos(const _float& fTimeDelta)
//{
//
//	_float angle = (_float)rand() / RAND_MAX * 2.f * D3DX_PI;
//	_float distance = 1.f + (rand() % 50); // 최소 1.f부터 최대 50.f까지의 거리를 랜덤하게 선택
//
//	m_vRandomPos.x = distance * cosf(angle);
//	m_vRandomPos.y = 1.f;
//	m_vRandomPos.z = -distance * sinf(angle);
//
//	D3DXVec3Normalize(&m_vRandomPos, &m_vRandomPos);
//
//	CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
//	_vec3 vRandomDir = Get_RandomDir(fTimeDelta);
//	D3DXVec3Normalize(&vRandomDir, &vRandomDir);
//	_vec3 pTarget = pPlayerTransform->m_vInfo[INFO_POS] + vRandomDir * 200 * fTimeDelta;
//
//	MoveTo_Pos(pTarget, fTimeDelta);
//}

//void CMonster_Move::Move_NewPos(const _float& fTimeDelta)
//{
//	_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
//	_vec3 vDir = m_vRandomPos - vCurrentPos;
//
//	D3DXVec3Normalize(&vDir, &vDir);
//
//	_float fMoveSpeed = 3.f;
//
//	_float fMoveDistanve = fMoveSpeed * fTimeDelta;
//
//	vCurrentPos += vDir * fMoveDistanve;
//
//	m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = vCurrentPos;
//}

//_bool CMonster_Move::Reached_Pos()
//{
//	_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
//	_float fDistance = D3DXVec3Length(&(vCurrentPos - m_vRandomPos));
//
//	return fDistance <= 70.f;
//}

void CMonster_Move::Move_RandomPos(const _float& fTimeDelta)
{
	//_vec3 vTerrainCenter = _vec3((VTXCNTX * 32) / 2.f, 0.f, (VTXCNTZ * 32) / 2.f);

	_float	fMinDistance = 10.f;
	_float	fMaxDistance = 40.f;

	_float fDistance = fMinDistance + (rand() / (_float)RAND_MAX) * (fMaxDistance - fMinDistance);

	//_vec3 vRamdomDir = _vec3(cosf(fAngle), 0.f, -sinf(fAngle));
	_vec3 vRandomDir = Get_RandomDir(fTimeDelta);
	m_vSavePos = vRandomDir;
	D3DXVec3Normalize(&vRandomDir, &vRandomDir);

	m_vRandomPos =  m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS] + vRandomDir * fDistance;

	MoveTo_Pos(m_vRandomPos, fTimeDelta);
}

_vec3 CMonster_Move::Get_RandomDir(const _float& fTimeDelta)
{
	_float distance = 15.f;

	if (dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_BlockOn())
	{
		distance *= -1.f;
		dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_BlockOn(false);
	}

	_float X = m_vSavePos.x + (distance * cosf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 100.f;
	_float Z = m_vSavePos.z + (distance * -sinf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 100.f;

	_vec3 vDir = _vec3(X, 0.f, Z);

	D3DXVec3Normalize(&vDir, &vDir);


	return vDir;

}

void CMonster_Move::MoveTo_Pos(const _vec3& vTargetPos, const _float& fTimeDelta)
{
	_vec3& vMonsterPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

	_vec3 vDir = vTargetPos - vMonsterPos;
	D3DXVec3Normalize(&vDir, &vDir);

	_float fMoveSpeed = 3.f;
	_float fMoveDistance = fMoveSpeed * fTimeDelta;

	vMonsterPos += vDir * fMoveDistance;
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


void CMonster_Move::Free()
{
	__super::Free();
}
