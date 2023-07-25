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
	m_bCheck = false;


	return S_OK;
}

STATE CMonster_Fly::Update_State(const _float& fTimeDelta)
{
	if (m_pOwner->Get_PrevState() == STATE::ATTACK || m_pOwner->Get_PrevState() == STATE::HIT)
		m_bAttackCoolDown = true;


	CTransform* pPlayerTransform = SceneManager()->
		Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

	_vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];

	_float fDistance = D3DXVec3LengthSq(&(vPlayerPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS]));
	_float fSight = pow(15, 2);


	if (fDistance >= fSight)
	{
		Fly(fTimeDelta);

		m_pOwner->Set_State(STATE::ROMIMG);
		return STATE::ROMIMG;
	}

	if (m_bFirstCool)
	{
		m_bFirstCool = false;
		dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_State(STATE::ATTACK);
		return STATE::ATTACK;
	}

	if (m_bAttackCoolDown)
	{
		m_fChase += fTimeDelta;

		if (m_fChase >= m_fAttackDuration)
		{
			m_bAttackCoolDown = false;
			m_fChase = 0.f;
			dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_State(STATE::ATTACK);
			dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
			return STATE::ATTACK;
		}
		else
		{
			Fly(fTimeDelta);
			m_pOwner->Set_State(STATE::ROMIMG);
			return STATE::ROMIMG;
		}
	
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

void CMonster_Fly::Fly(const _float& fTimeDelta)
{
	//srand((unsigned)time(NULL));
	const _float fMinDistance = 10.f;
	const _float fMaxDistance = 270.f;
	const _float fAmplitude = 8.f;

	//_float fRandX = 1 + rand() % 250;
	//_float fRandY = 1 + rand() % 250;
	//_float fRandZ = 1 + rand() % 250;

	_float X = m_vSavePos.x + (fAmplitude * cosf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 100.f;
	_float Y = m_vSavePos.y + (fAmplitude * -sinf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 100.f;
	_float Z = m_vSavePos.z + (-fAmplitude * cosf((_float)rand() / RAND_MAX * 2.f * D3DX_PI)) / 100.f;

	_vec3 vDir = _vec3(X, Y, Z);

	D3DXVec3Normalize(&vDir, &vDir);

	m_vSavePos = vDir;

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	_vec3 vTargetPos = dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_CenterPos() - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

	_float fReturnDistance = D3DXVec3Length(&vTargetPos);
	D3DXVec3Normalize(&vTargetPos, &vTargetPos);

	if (fReturnDistance >= dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_MoveRange() &&
		!m_bCheck)
	{
		_float fPlayerDistance = D3DXVec3Length(&(rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS]));

		_vec3 vChase = rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS];

		D3DXVec3Normalize(&vChase, &vChase);


		if (fPlayerDistance < 50.f)
		{
			_vec3 vTest = _vec3(vDir.x, 0.f, vDir.z);
			_vec3 vRan = vChase + vTest * 3.f;
			D3DXVec3Normalize(&vRan, &vRan);
			m_pOwner->Get_Transform()->Translate(vRan * 4.f * fTimeDelta);

			return;
		}


		D3DXVec3Normalize(&vTargetPos, &vTargetPos);
		m_pOwner->Get_Transform()->Translate(vTargetPos * 4.f * fTimeDelta);

		return;
	}
	else
		m_bCheck = true;

	if (m_bCheck)
	{
		if (fReturnDistance > dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_RandomoMoveRange())
			m_bCheck = false;
		else
		{
			vTargetPos = m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS] + vDir * 400.f;

			_vec3 vDir2 = vTargetPos - m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&vDir2, &vDir2);

			_float fMoveSpeed = dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_BasicStat()->Get_Stat()->fSpeed;
			_float fMoveDistance = fMoveSpeed * fTimeDelta;

			_vec3 vNewPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + vDir2 * fMoveDistance;

			if (vNewPos.y < fMinDistance)
				vNewPos.y = fMinDistance;

			// 하늘의 최대 높이를 넘지 않도록 제한
			if (vNewPos.y > fMaxDistance)
				vNewPos.y = fMaxDistance;

			m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = vNewPos;
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
