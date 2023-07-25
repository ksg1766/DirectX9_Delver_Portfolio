#include "..\Header\Monster_Jump.h"
#include "Export_Function.h"
#include "DungeonSpider.h"
#include "Worm.h"

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
	m_fJumpVelocity = 8.f;
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
	
	_vec3& vMonsterPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

	CRigidBody* pRigidBody = nullptr;

	if (dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Get_MonsterTag() == MONSTERTAG::SPIDER)
		pRigidBody = dynamic_cast<CDungeonSpider*>(m_pOwner->Get_Host())->Get_RigidBody();


	if (!dynamic_cast<CMonster*>(m_pOwner->Get_Host())->IsJump())
	{
		m_vLastPos = pPlayerTransform->m_vInfo[INFO_POS];
		//dynamic_cast<CDungeonSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->Add_Force(_vec3(0.f, 20.f, 0.f));
		//dynamic_cast<CDungeonSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->UseGravity(true);
		//dynamic_cast<CDungeonSpider*>(m_pOwner->Get_Host())->Set_Jump(true);
	}
	
	if (dynamic_cast<CMonster*>(m_pOwner->Get_Host())->IsJump())
	{
		_vec3 vDir = m_vLastPos - vMonsterPos;
		m_pOwner->Get_Host()->m_pTransform->Translate(_vec3(vDir.x, 0.f, vDir.z) * 5.f * fTimeDelta);
	}

	if (!dynamic_cast<CMonster*>(m_pOwner->Get_Host())->IsJump())
	{
		m_pOwner->Get_Animator()->Get_Animation()->Set_Frame(0.f);

		return STATE::ROMIMG;
	}
	
	m_pOwner->Get_Animator()->Get_Animation()->Set_Frame(0.f);
	return STATE::ATTACK;

	//if (!dynamic_cast<CMonster*>(m_pOwner->Get_Host())->IsJump())
	//{
	//	m_fChase += fTimeDelta;

	//	if (m_fChase < 0.1f)
	//	{
	//		m_pOwner->Get_Animator()->Get_Animation()->Set_Frame(1.f);
	//		m_pOwner->Get_Animator()->Get_Animation()->Set_Loop(false);
	//	}
	//	else
	//	{
	//		dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
	//		m_pOwner->Get_Animator()->Get_Animation()->Set_Loop(true);
	//		m_vLastPos = pPlayerTransform->m_vInfo[INFO_POS];
	//		//m_bIsJumping = true;
	//		dynamic_cast<CDungeonSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->Add_Force(_vec3(0.f, 3.f * 4, 0.f));
	//		dynamic_cast<CDungeonSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->UseGravity(true);
	//		
	//
	//		m_fChase = 0.f;
	//	}
	//}

	//if (dynamic_cast<CMonster*>(m_pOwner->Get_Host())->IsJump())
	//{
	//	_vec3 vDir = m_vLastPos - vMonsterPos;
	//	D3DXVec3Normalize(&vDir, &vDir);

	//	//vMonsterPos.y += m_fJumpVelocity * fTimeDelta;
	//	//vMonsterPos += vDir * 10.f * fTimeDelta;
	//	m_pOwner->Get_Host()->m_pTransform->Translate(_vec3(vDir.x, 0.f, vDir.z) * 10.f * fTimeDelta);

	//	//m_fJumpVelocity -= 0.5f * fTimeDelta * fTimeDelta * 3000.f;
	//}


	//if (!dynamic_cast<CMonster*>(m_pOwner->Get_Host())->IsJump())
	//{
	//	//m_fJumpVelocity = 8.f;
	//	m_bJumCoolDown = true;
	//	m_bIsJumping = false;
	//	//dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_Jump(false);
	//	m_pOwner->Get_Animator()->Get_Animation()->Set_Frame(0.f);
	//	

	//	return STATE::ROMIMG;
	//}

	////m_pOwner->Get_Animator()->Get_Animation()->Set_Frame(0.f);

	//return STATE::ATTACK;
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
