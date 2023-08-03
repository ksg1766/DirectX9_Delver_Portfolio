#include "stdafx.h"
#include "KingSpider.h"
#include "Export_Function.h"
#include "Player.h"
#include "PoolManager.h"
#include "SoundManager.h"
#include "KingSpider_Appear.h"
#include "KingSpider_Idle.h"
#include "KingSpider_Run.h"
#include "KingSpider_Idle.h"
#include "KingSpider_Shoot.h"
#include "KingSpider_Jump.h"
#include "KingSpider_ShootPoison.h"
#include "KingSpider_MakeNest.h"
#include "KingSpider_Dead.h"

CKingSpider::CKingSpider(LPDIRECT3DDEVICE9 pGrapicDev)
	: Engine::CMonster(pGrapicDev)
{
}

CKingSpider::CKingSpider(const CKingSpider& rhs)
	: Engine::CMonster(rhs)
{
}

CKingSpider::~CKingSpider()
{
}

HRESULT CKingSpider::Ready_Object()
{
	Set_ObjectTag(OBJECTTAG::BOSS);
	Set_MonsterState(MONSTERTAG::SPIDER);
	m_ePhase = BOSSPHASE::PHASE1;
	m_bJumpRun = false;
	m_bFloorCollison = false;
	m_fFloorHeight = 0.f;
	m_iCrawlingHP = 10;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pState = CKingSpider_Appear::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_WAKEUP, m_pState);

	m_pState = CKingSpider_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_IDLE, m_pState);

	m_pState = CKingSpider_Shoot::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL1, m_pState);

	m_pState = CKingSpider_ShootPoison::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL2, m_pState);

	m_pState = CKingSpider_Jump::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL3, m_pState);

	m_pState = CKingSpider_MakeNest::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL4, m_pState);

	m_pState = CKingSpider_Run::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_TELEPORT, m_pState);

	m_pState = CKingSpider_Dead::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_DEAD, m_pState);

	//////////////////////////////////////////////
	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ROMIMG], STATE::BOSS_WAKEUP, 10.f, FALSE);
	m_pAnimator->Add_Animation(STATE::BOSS_WAKEUP, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ROMIMG], STATE::BOSS_IDLE, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_IDLE, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ATTACK], STATE::BOSS_PH1SKILL1, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL1, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ATTACK], STATE::BOSS_PH1SKILL2, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL2, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ATTACK], STATE::BOSS_PH1SKILL3, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL3, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ATTACK], STATE::BOSS_PH1SKILL4, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL4, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::DEAD], STATE::BOSS_DEAD, 10.f, FALSE);
	m_pAnimator->Add_Animation(STATE::BOSS_DEAD, pAnimation);

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::BOSS_WAKEUP);

	m_pTransform->Scale(_vec3(4.f, 4.f, 4.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	m_pRigidBody->UseGravity(false);
	Init_Stat();
	return S_OK;
}

_int CKingSpider::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this); 
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);

	m_pStateMachine->Update_StateMachine(fTimeDelta);
	
	if ((14.99f >= m_pTransform->m_vInfo[INFO_POS].y))
		m_pTransform->m_vInfo[INFO_POS].y = 15.f;

	return iExit;
}

void CKingSpider::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(4.f, 4.f, 4.f));
}

void CKingSpider::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();
#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CKingSpider::Init_Stat()
{
	m_pBasicStat->Get_Stat()->fMaxHP = 60.f;
	m_pBasicStat->Get_Stat()->fHP = 60.f;
	m_pBasicStat->Get_Stat()->fDeffense = 4.f;
	m_pBasicStat->Get_Stat()->fAttack = 4.f;
	m_pBasicStat->Get_Stat()->iArmorMin = 0.f;
	m_pBasicStat->Get_Stat()->iArmorMax = 3.f;
	m_pBasicStat->Get_Stat()->iExp = 15.f;

}

void CKingSpider::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if(OBJECTTAG::PLAYERBULLET != _pOther->Get_Host()->Get_ObjectTag())
	__super::OnCollisionEnter(_pOther);
	if (OBJECTTAG::PLAYERBULLET == _pOther->Get_Host()->Get_ObjectTag())
	{
		if (STATE::BOSS_TELEPORT == m_pStateMachine->Get_State())
			--m_iCrawlingHP;
		if (0 >= m_iCrawlingHP)
		{
			m_pRigidBody->UseGravity(true);
		}
	}
}

void CKingSpider::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (OBJECTTAG::PLAYERBULLET != _pOther->Get_Host()->Get_ObjectTag())
		__super::OnCollisionEnter(_pOther);
	if (OBJECTTAG::BLOCK == _pOther->Get_Host()->Get_ObjectTag())
	{
		if (STATE::BOSS_TELEPORT == m_pStateMachine->Get_State())
			m_bFloorCollison = true;

	}
}

void CKingSpider::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
		__super::OnCollisionExit(_pOther);
		if (OBJECTTAG::BLOCK == _pOther->Get_Host()->Get_ObjectTag())
		{
			if (STATE::BOSS_TELEPORT == m_pStateMachine->Get_State())
				m_bFloorCollison = false;
		}
}

HRESULT CKingSpider::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture[(_uint)STATE::ROMIMG] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_KingSpider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::ATTACK] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_KingSpiderAttack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::HIT] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_KingSpiderHit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::DEAD] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_KingSpiderDead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pStateMachine = dynamic_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CKingSpider* CKingSpider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CKingSpider* pInstance = new CKingSpider(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CKingSpider*>(pInstance);

		MSG_BOX("Spiedt Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CKingSpider::Free()
{
	Safe_Release(m_pState);
	__super::Free();
}
