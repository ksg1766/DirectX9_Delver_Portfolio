#include "..\Header\Monster_Jump.h"
#include "Export_Function.h"
#include "DungeonSpider.h"

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
	m_fJumpVelocity = 13.f;
	m_bJumCoolDown = false;
	m_bIsJumping = false;
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

	CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
	

	_vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];
	_vec3& vMonsterPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

	if (!m_bIsJumping)
	{
		m_vLastPos = vPlayerPos;
		m_bIsJumping = true;
	}

	_vec3 vDir = m_vLastPos - vMonsterPos;
	D3DXVec3Normalize(&vDir, &vDir);

	vMonsterPos.y += m_fJumpVelocity * fTimeDelta;
	vMonsterPos += vDir * 10.f * fTimeDelta;

	m_fJumpVelocity -= 0.5f * fTimeDelta * fTimeDelta * 3000.f;

	_vec3 vDistance = vPlayerPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	_float fDistanceLength = D3DXVec3LengthSq(&vDistance);
	_float fSight = pow(15, 2);


	if (vMonsterPos.y < 1.f)
	{
		vMonsterPos.y = 1.f;
		m_fJumpVelocity = 13.f;
		m_bJumCoolDown = true;
		m_bIsJumping = false;
		
	
		m_pOwner->Get_Host()->Set_State(STATE::ROMIMG);
		
		return STATE::ROMIMG;
	}


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
