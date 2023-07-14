#include "..\Header\Monster_Move.h"
#include "Export_Function.h"
#include "DungeonSpider.h"

CMonster_Move::CMonster_Move()
{
}

CMonster_Move::CMonster_Move(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev), m_vSavePos(_vec3(0.f,0.f,0.f))
{
}

CMonster_Move::~CMonster_Move()
{
}

HRESULT CMonster_Move::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_bJumCoolDown = false;
	m_bFirstCool = true;
	m_fJumpCoolDuration = 2.f;
		m_fJumpCoolTimer = 0.f;
		m_fAmplitude = 0.f;
	m_vReverseDir = _vec3(0.f, 0.f, 0.f);
	return S_OK;
}


STATE CMonster_Move::Update_State(const _float& fTimeDelta)
{

	if (m_pOwner->Get_PrevState() == STATE::ATTACK || m_pOwner->Get_PrevState() == STATE::HIT)
		m_bJumCoolDown = true;

	CComponent* pPlayerTransform =
		SceneManager()->GetInstance()->
		Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()
		->Get_Component(COMPONENTTAG::TRANSFORM, COMPONENTID::ID_DYNAMIC);

	_vec3 vPlayerPos = static_cast<CTransform*>(pPlayerTransform)->m_vInfo[INFO_POS];

	_vec3 vDistance = vPlayerPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	_float fDistanceLength = D3DXVec3LengthSq(&vDistance);
	_float fSight = pow(13, 2);


	if (fDistanceLength >= fSight) // 몬스터의 시야보다 플레이어와 몬스터의 사이거리가 멀 때까지 true
	{
		if (dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_WallTouch()) // 사이가 멀다면, 현재 비교대상과의 거리를 측정하고 true, false값을 반환시키고 할 행동을한다..
		{
			Set_NewPos(); // 비교대상과 가까워졌다면 새로운 Pos 값을 줘야되니까, 그 값을 줬다면 Move_NewPos -> 새로운 곳으로 이동시킴.
			//dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_WallTouch(false);
		}
		
		Move_RandomPos(fTimeDelta);

		return STATE::ROMIMG;
	}



	if (m_bFirstCool)
	{
		m_bFirstCool = false;
		return STATE::ATTACK;
	}


	if (m_bJumCoolDown)
	{
		m_fJumpCoolTimer += fTimeDelta;

		if (m_fJumpCoolTimer >= m_fJumpCoolDuration)
		{
			m_bJumCoolDown = false;
			m_fJumpCoolTimer = 0.f;
			dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
			return STATE::ATTACK;
		}
		else
		{
			Move_RandomPos(fTimeDelta);
		}
	}

	m_pOwner->Set_State(STATE::ROMIMG);

}

void CMonster_Move::Set_NewPos()
{
	srand((unsigned)time(NULL));


	//_float radius = 10.f; // X와 Z 값의 최대 범위
	//_float angle = (_float)rand() / 2.f * D3DX_PI; // 0에서 2파이(360도) 사이의 랜덤한 각도
	//
	//_float X = m_vSavePos.x + (radius * cosf(angle)) / 100.f; // X 값 계산
	//_float Z = m_vSavePos.z + (radius * -sinf(angle)) / 100.f; // Z 값 계산

	/*m_vRandomPos.x = 15 * cosf((_float)rand() / RAND_MAX * (D3DX_PI * 2));
	m_vRandomPos.y = 1.f;
	m_vRandomPos.z = 15 * -sinf((_float)rand() / RAND_MAX * (D3DX_PI * 2));*/

	m_vRandomPos *= -1.f;

	m_fAmplitude = 50 + rand() % 30;

	_float fRadius = 5.f;
	_float fAngle = (_float)rand() / 2.f * D3DX_PI;
	
	m_vRandomPos.x = (fRadius * cosf(fAngle)) / m_fAmplitude;
	m_vRandomPos.y = 1.f;
	m_vRandomPos.z = (fRadius * -sinf(fAngle)) / m_fAmplitude;
}

void CMonster_Move::Move_NewPos(const _float& fTimeDelta)
{
	_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	_vec3 vDir = (_vec3(0.f,0.f,0.f)) - vCurrentPos;

	D3DXVec3Normalize(&vDir, &vDir);

	_float fMoveSpeed = 5.f;

	_float fMoveDistanve = fMoveSpeed * fTimeDelta;

	vCurrentPos += vDir * fMoveDistanve;

	//m_pOwner->Get_Transform()->Translate(vCurrentPos * fTimeDelta);

	m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = vCurrentPos;
}

_bool CMonster_Move::Reached_Pos()
{
	_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&(vCurrentPos - _vec3(0.f,0.f,0.f)));

	return fDistance <= 20.f;
}

void CMonster_Move::Move_RandomPos(const _float& fTimeDelta)
{
	CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

	_vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];
	_vec3 vRandomDir = Get_RandomDir(fTimeDelta);
	m_vSavePos = vRandomDir;
	_vec3 vTargetPos = _vec3(0.f,0.f,0.f) + vRandomDir * 200.f;
	
	//m_vReverseDir = -(vTargetPos + vRandomDir * 200.f);

	MoveTo_Pos(vTargetPos, fTimeDelta);
}

_vec3 CMonster_Move::Get_RandomDir(const _float& fTimeDelta)
{

	_float radius = 10.f; // X와 Z 값의 최대 범위
	_float angle = (_float)rand() / 2.f * D3DX_PI; // 0에서 2파이(360도) 사이의 랜덤한 각도

	_float X = m_vSavePos.x + (radius * cosf(angle)) / 100.f; // X 값 계산
	_float Z = m_vSavePos.z + (radius * -sinf(angle)) / 100.f; // Z 값 계산

	m_vRandomPos = _vec3(X, 0.f, Z);
	D3DXVec3Normalize(&m_vRandomPos, &m_vRandomPos);


	return m_vRandomPos;

}

void CMonster_Move::MoveTo_Pos(const _vec3& vTargetPos, const _float& fTimeDelta)
{
	_vec3& vMonsterPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

	_vec3 vDir = vTargetPos - vMonsterPos;
	D3DXVec3Normalize(&vDir, &vDir);

	_float fMoveSpeed = 5.f;
	_float fMoveDistance = fMoveSpeed * fTimeDelta;

	vMonsterPos += vDir * fMoveDistance;
}





// 잠시 test





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
//
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
//
//_bool CMonster_Move::Reached_Pos()
//{
//	_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
//	_float fDistance = D3DXVec3Length(&(vCurrentPos - m_vRandomPos));
//
//	return fDistance <= 70.f;
//}
//
//void CMonster_Move::Move_RandomPos(const _float& fTimeDelta)
//{
//	//_vec3 vTerrainCenter = _vec3((VTXCNTX * 32) / 2.f, 0.f, (VTXCNTZ * 32) / 2.f);
//	_vec3 vTerrainCenter = _vec3(0.f, 0.f, 0.f);
//
//	_float	fMinDistance = 10.f;
//	_float	fMaxDistance = 100.f;
//
//	_float fDistance = fMinDistance + (rand() / (_float)RAND_MAX) * (fMaxDistance - fMinDistance);
//
//	//_vec3 vRamdomDir = _vec3(cosf(fAngle), 0.f, -sinf(fAngle));
//	_vec3 vRandomDir = Get_RandomDir(fTimeDelta);
//	m_vSavePos = vRandomDir * 300;
//	D3DXVec3Normalize(&vRandomDir, &vRandomDir);
//
//	m_vRandomPos =  vTerrainCenter + vRandomDir * fDistance;
//
//	MoveTo_Pos(m_vRandomPos, fTimeDelta);
//}
//
//_vec3 CMonster_Move::Get_RandomDir(const _float& fTimeDelta)
//{
//	_float distance = 1.f + (rand() % 50);
//
//
//	_float X = m_vSavePos.x + (distance * cosf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 100.f;
//	_float Z = m_vSavePos.z + (-distance * sinf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 100.f;
//
//	_vec3 vDir = _vec3(X, 0.f, Z);
//
//	D3DXVec3Normalize(&vDir, &vDir);
//
//
//	return vDir;
//
//}
//
//void CMonster_Move::MoveTo_Pos(const _vec3& vTargetPos, const _float& fTimeDelta)
//{
//	_vec3& vMonsterPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
//
//	_vec3 vDir = vTargetPos - vMonsterPos;
//	D3DXVec3Normalize(&vDir, &vDir);
//
//	_float fMoveSpeed = 3.f;
//	_float fMoveDistance = fMoveSpeed * fTimeDelta;
//
//	vMonsterPos += vDir * fMoveDistance;
//}


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
