#include "..\Header\Monster_Move.h"
#include "Export_Function.h"

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
	//m_eState = STATE::ROMIMG;

	return S_OK;
}

STATE CMonster_Move::Update_State(const _float& fTimeDelta)
{
	
	CComponent* pPlayerTransform =
		SceneManager()->GetInstance()->
		Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()
		->Get_Component(COMPONENTTAG::TRANSFORM, COMPONENTID::ID_DYNAMIC);

	_vec3 vPlayerPos = static_cast<CTransform*>(pPlayerTransform)->m_vInfo[INFO_POS];

	_vec3 vDistance = vPlayerPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	_float fDistanceLength = D3DXVec3LengthSq(&vDistance);
	_float fSight = pow(15, 2);

	m_fChase += fTimeDelta;

	if (fDistanceLength >= fSight)
	{
		if (Reached_Pos())
			Set_NewPos();
		else
			Move_NewPos(fTimeDelta);

		return STATE::ROMIMG;
	}
	else
		return STATE::ATTACK;
}

void CMonster_Move::LateUpdate_State()
{
}

void CMonster_Move::Render_State()
{
	cout << "Spider Move" << endl;
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

void CMonster_Move::Set_NewPos()
{
	srand((unsigned)time(NULL));

	m_vRandomPos.x = 15 * cosf((_float)rand() / 30 * (D3DX_PI * 2));
	m_vRandomPos.y = 1.f;
	m_vRandomPos.z = 15 * -sinf((_float)rand() / 30 * (D3DX_PI * 2));

}

void CMonster_Move::Move_NewPos(const _float& fTimeDelta)
{
	_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	_vec3 vDir = m_vRandomPos - vCurrentPos;

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
	_float fDistance = D3DXVec3Length(&(vCurrentPos - m_vRandomPos));

	return fDistance <= 5.f;
}

void CMonster_Move::Free()
{
	__super::Free();
}
