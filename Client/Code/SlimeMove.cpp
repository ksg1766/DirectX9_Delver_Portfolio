#include "..\Header\SlimeMove.h"
#include "Export_Function.h"
#include "DungeonSpider.h"

CSlimeMove::CSlimeMove()
{
}

CSlimeMove::CSlimeMove(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev), m_vSavePos(_vec3(0.f, 0.f, 0.f))
{
}

CSlimeMove::~CSlimeMove()
{
}

HRESULT CSlimeMove::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_bJumCoolDown = false;
	m_bFirstCool = true;
	m_fJumpCoolDuration = 2.f;
	m_fJumpCoolTimer = 0.f;

	return S_OK;
}

STATE CSlimeMove::Update_State(const _float& fTimeDelta)
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
	_float fSight = pow(4, 2);


	if (fDistanceLength >= fSight)
	{
		if (Reached_Pos())
			Set_NewPos();
		else
			Move_NewPos(fTimeDelta);


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

void CSlimeMove::LateUpdate_State()
{
}

void CSlimeMove::Render_State()
{
	//cout << "Spider Move" << endl;
}

CSlimeMove* CSlimeMove::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CSlimeMove* pState = new CSlimeMove(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CSlimeMove*>(pState);

		MSG_BOX("Create Monster Move Failed");
		return nullptr;
	}

	return pState;
}

void CSlimeMove::Set_NewPos()
{
	srand((unsigned)time(NULL));

	m_vRandomPos.x = 15 * cosf((_float)rand() / 30 * (D3DX_PI * 2));
	m_vRandomPos.y = 1.f;
	m_vRandomPos.z = 15 * -sinf((_float)rand() / 30 * (D3DX_PI * 2));

}

void CSlimeMove::Move_NewPos(const _float& fTimeDelta)
{
	_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	_vec3 vDir = m_vRandomPos - vCurrentPos;

	D3DXVec3Normalize(&vDir, &vDir);

	_float fMoveSpeed = 3.f;

	_float fMoveDistanve = fMoveSpeed * fTimeDelta;

	vCurrentPos += vDir * fMoveDistanve;

	//m_pOwner->Get_Transform()->Translate(vCurrentPos * fTimeDelta);

	m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = vCurrentPos;
}

_bool CSlimeMove::Reached_Pos()
{
	_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&(vCurrentPos - m_vRandomPos));

	return fDistance <= 5.f;
}

void CSlimeMove::Move_RandomPos(const _float& fTimeDelta)
{
	CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

	_vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];
	_vec3 vRandomDir = Get_RandomDir(fTimeDelta);
	m_vSavePos = vRandomDir;
	_vec3 vTargetPos = vPlayerPos + vRandomDir * 80.f;

	MoveTo_Pos(vTargetPos, fTimeDelta);
}

_vec3 CSlimeMove::Get_RandomDir(const _float& fTimeDelta)
{
	_float X = m_vSavePos.x + (15 * cosf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 50.f;
	_float Z = m_vSavePos.z + (15 * -sinf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 50.f;
	//_float angle = (_float)rand() / RAND_MAX * 2.f * D3DX_PI;
	//m_vRandomPos = _vec3(cosf(angle), 1.f, -sinf(angle));

	_vec3 vDir = _vec3(X, 0.f, Z);

	D3DXVec3Normalize(&vDir, &vDir);


	return vDir;

}

void CSlimeMove::MoveTo_Pos(const _vec3& vTargetPos, const _float& fTimeDelta)
{
	_vec3& vMonsterPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

	_vec3 vDir = vTargetPos - vMonsterPos;
	D3DXVec3Normalize(&vDir, &vDir);

	_float fMoveSpeed = 3.f;
	_float fMoveDistance = fMoveSpeed * fTimeDelta;

	vMonsterPos += vDir * fMoveDistance;
}

void CSlimeMove::Free()
{
	__super::Free();
}
