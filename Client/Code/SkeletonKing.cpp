#include "SkeletonKing.h"
#include "Export_Function.h"
#include "Boss_Sleep.h"
#include "Boss_WakeUp.h"
#include "Boss_Idle.h"
#include "Boss_Phase1.h"
#include "Boss_Phase2.h"
#include "Boss_Phase3.h"
#include "FirePattern.h"
#include "ExplosionPattern.h"
#include "TeleportPattern.h"
#include "Boss_SkeletonSpawnPattern.h"
#include "CrawlPattern.h"
#include "Boss_Sturn.h"
#include "Boss_Meteor1Ph.h"
#include "Boss_MeteorReady.h"
#include "Boss_Dying.h"
#include "Boss_Dead.h"
#include "FireWallPttern.h"
#include "FireWavePattern.h"
#include "Clone_Pattern.h"
#include "LostSoulPattern.h"
#include "Boss_LostSoul.h"
#include "GrapPattern.h"
#include "CrossPattern.h"
#include "MeteorPh2.h"
#include "FIreWave2.h"
#include "LostSoulPattern2.h"
#include "LightningPattern2.h"
#include "MiniMetorPattern.h"
#include "MeteorPh3.h"
#include "Player.h"
#include "Boss_LastPhase.h"
#include "SkeletonKing_Clone.h"
CSkeletonKing::CSkeletonKing(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{

}

CSkeletonKing::CSkeletonKing(const CSkeletonKing& rhs)
	: Engine::CMonster(rhs)
{
}

CSkeletonKing::~CSkeletonKing()
{
}

HRESULT CSkeletonKing::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BOSS;
	m_eMonsterTag = MONSTERTAG::SKELETON;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pRigidBody->UseGravity(false);
	m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));
	m_pBasicStat->Get_Stat()->fHP = 100.f;
	m_ePhase = BOSSPHASE::PHASE1;
	m_iHitCount = 0;
	m_fHitCool = 0.f;
	m_fMoveDelay = 0.f;
	m_fDelay = 0.f;
	m_bSturn = false;
	m_b3Phase = false;
	m_bMove = false;
	m_iCloneCount = 0;
#pragma region 상태

	//상태추가
	CState* m_pState = CBoss_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_IDLE, m_pState);

	m_pState = CBoss_Sleep::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_SLEEP, m_pState);

	m_pState = CBoss_WakeUp::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_WAKEUP, m_pState);
#pragma region 1페
	m_pState = CBoss_Phase1::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_ATTACK1, m_pState);

	m_pState = CFirePattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL1, m_pState);

	m_pState = CExplosionPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL2, m_pState);

	m_pState = CBoss_SkeletonSpawnPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL3, m_pState);

	m_pState = CFireWallPttern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL4, m_pState);

	m_pState = CBoss_MeteorPh1::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL5, m_pState);
#pragma endregion 1페

#pragma region 2페
	m_pState = CBoss_Phase2::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_ATTACK2, m_pState);

	m_pState = CClone_Pattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH2SKILL1, m_pState);

	m_pState = CCrossPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH2SKILL2, m_pState);

	m_pState = CFireWavePattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH2SKILL3, m_pState);

	m_pState = CLostSoulPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH2SKILL4, m_pState);

	m_pState = CBoss_Meteor2Ph::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH2SKILL5, m_pState);
#pragma endregion 2페

#pragma region 3페
	m_pState = CBoss_Phase3::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_ATTACK3, m_pState);

	m_pState = CFIreWave2::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH3SKILL1, m_pState);

	m_pState = CLostSoulPattern2::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH3SKILL2, m_pState);

	m_pState = CLightningPattern2::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH3SKILL3, m_pState);

	m_pState = CMiniMetorPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH3SKILL4, m_pState);

	m_pState = CMeteorPh3::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH3SKILL5, m_pState);
#pragma endregion 3페


	m_pState = CTeleportPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_TELEPORT, m_pState);

	//m_pState = CCrawlPattern::Create(m_pGraphicDev, m_pStateMachine);
	//m_pStateMachine->Add_State(STATE::BOSS_CRAWL, m_pState);

	m_pState = CBoss_LastPhase::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_CRAWL, m_pState);

	m_pState = CBoss_Sturn::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_STURN, m_pState);

	m_pState = CBoss_MeteorReady::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_METEORREADY, m_pState);

	m_pState = CBoss_Dying::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_DYING, m_pState);

	m_pState = CBoss_Dead::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_DEAD, m_pState);

#pragma endregion 상태

#pragma region 애니메이션

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_IDLE], STATE::BOSS_IDLE, 12.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_IDLE, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_SLEEP], STATE::BOSS_SLEEP, 8.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_SLEEP, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_WAKEUP], STATE::BOSS_WAKEUP, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_WAKEUP, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_IDLE], STATE::BOSS_ATTACK1, 12.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_ATTACK1, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_IDLE], STATE::BOSS_ATTACK2, 12.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_ATTACK2, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_IDLE], STATE::BOSS_ATTACK3, 12.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_ATTACK3, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH1SKILL1, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL1, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH1SKILL2, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL2, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH1SKILL3, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL3, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH1SKILL4, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL4, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH1SKILL5, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL5, pAnimation);
	
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH2SKILL1, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH2SKILL1, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH2SKILL2, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH2SKILL2, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH2SKILL3, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH2SKILL3, pAnimation);

		pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH2SKILL4, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH2SKILL4, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH2SKILL5, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH2SKILL5, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH3SKILL1, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH3SKILL1, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH3SKILL2, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH3SKILL2, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH3SKILL3, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH3SKILL3, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH3SKILL4, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH3SKILL4, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH3SKILL5, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH3SKILL5, pAnimation);
	
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_TELEPORT, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_TELEPORT, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_CRAWL], STATE::BOSS_CRAWL, 20.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_CRAWL, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_STURN], STATE::BOSS_STURN, 2.f, FALSE);
	m_pAnimator->Add_Animation(STATE::BOSS_STURN, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_METEORREADY, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_METEORREADY, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_DYING], STATE::BOSS_DYING, 6.f, FALSE);
	m_pAnimator->Add_Animation(STATE::BOSS_DYING, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_DEAD], STATE::BOSS_DEAD, 0.f, FALSE);
	m_pAnimator->Add_Animation(STATE::BOSS_DEAD, pAnimation);

#pragma endregion 애니메이션

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::BOSS_SLEEP);
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	return S_OK;
}

_int CSkeletonKing::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	if ((STATE::BOSS_PH2SKILL1 == m_pStateMachine->Get_State()) || (STATE::BOSS_SLEEP != m_pStateMachine->Get_State()) && ((STATE::BOSS_STURN != m_pStateMachine->Get_State())))
		m_pRigidBody->UseGravity(false); 

	else if((STATE::BOSS_PH2SKILL1 != m_pStateMachine->Get_State())&&(!m_b3Phase)&&(STATE::BOSS_SLEEP == m_pStateMachine->Get_State())||((STATE::BOSS_STURN == m_pStateMachine->Get_State())))
		m_pRigidBody->UseGravity(true);

	_int iExit = __super::Update_Object(fTimeDelta);
	m_pStateMachine->Update_StateMachine(fTimeDelta);

	if (m_b3Phase)
		m_pTransform->m_vInfo[INFO_POS] = _vec3(-72.f, 34.f, -105.f);

	return iExit;
}

void CSkeletonKing::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();

	if((BOSSPHASE::PHASE3 == m_ePhase) && (m_b3Phase))
		m_pTransform->Scale(_vec3(12.f, 12.f, 12.f));
	else
		m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale() * 0.9f);
}

void CSkeletonKing::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CSkeletonKing::Init_Stat()
{
	m_pBasicStat->Get_Stat()->fHP = 100.f;
	m_pBasicStat->Get_Stat()->fDeffense = 3.f;
	m_pBasicStat->Get_Stat()->iArmorMin = 1;
	m_pBasicStat->Get_Stat()->iArmorMax = 6;
}

void CSkeletonKing::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if ((OBJECTTAG::PLAYERBULLET != _pOther->Get_Host()->Get_ObjectTag()))
	{
		if((STATE::BOSS_TELEPORT == m_pStateMachine->Get_State())||(STATE::BOSS_PH2SKILL1 == m_pStateMachine->Get_State())||(OBJECTTAG::PLAYER == _pOther->Get_Host()->Get_ObjectTag()))
			return;

		else if ((OBJECTTAG::LOSTSOUL == _pOther->Get_Host()->Get_ObjectTag()))
			if (SOULSTATE::SOUL_PARRY != dynamic_cast<CBossLostSoul*>(_pOther->Get_Host())->Get_SoulState())
				return;

		__super::OnCollisionEnter(_pOther);
	}
}

void CSkeletonKing::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if ((OBJECTTAG::PLAYERBULLET != _pOther->Get_Host()->Get_ObjectTag()))
	{
		if ((STATE::BOSS_TELEPORT == m_pStateMachine->Get_State()) || (STATE::BOSS_PH2SKILL1 == m_pStateMachine->Get_State()) || (OBJECTTAG::PLAYER == _pOther->Get_Host()->Get_ObjectTag()))
			return;

		else if ((OBJECTTAG::LOSTSOUL == _pOther->Get_Host()->Get_ObjectTag()))
			if (SOULSTATE::SOUL_PARRY != dynamic_cast<CBossLostSoul*>(_pOther->Get_Host())->Get_SoulState())
				return;

		__super::OnCollisionStay(_pOther);
	}
}

void CSkeletonKing::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if ((OBJECTTAG::PLAYERBULLET != _pOther->Get_Host()->Get_ObjectTag()))
	{
		if ((STATE::BOSS_TELEPORT == m_pStateMachine->Get_State()) || (STATE::BOSS_PH2SKILL1 == m_pStateMachine->Get_State()) || (OBJECTTAG::PLAYER == _pOther->Get_Host()->Get_ObjectTag()))
			return;

		else if ((OBJECTTAG::LOSTSOUL == _pOther->Get_Host()->Get_ObjectTag()))
			if (SOULSTATE::SOUL_PARRY != dynamic_cast<CBossLostSoul*>(_pOther->Get_Host())->Get_SoulState())
				return;

		__super::OnCollisionExit(_pOther);
	}
}

void CSkeletonKing::Add_CloneCount(_int _Hit)
{
	m_iCloneCount += _Hit; 
	if (0 >= m_iCloneCount)m_iCloneCount = 0;
}

void CSkeletonKing::Add_HitCount()
{
	if (m_eState == STATE::BOSS_STURN)
		return;
	++m_iHitCount;
	if (!m_b3Phase)
	{
		if (3 <= m_iHitCount)
		{
			m_iHitCount = 0.f;
			m_pStateMachine->Set_State(STATE::BOSS_STURN);
		}
	}
	if (m_b3Phase)
	{
		if (5 <= m_iHitCount)
		{
			m_iHitCount = 0.f;
			m_pStateMachine->Set_State(STATE::BOSS_STURN);
		}
	}
}

HRESULT CSkeletonKing::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

#pragma region 텍스쳐 컴포넌트

	pComponent = m_pTexture[(_uint)STATE::BOSS_IDLE] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Boss"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	
	pComponent = m_pTexture[(_uint)STATE::BOSS_ATTACK] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossAttack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::BOSS_STURN] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossHit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::BOSS_WAKEUP] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Boss_StandUp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::BOSS_DYING] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossDying"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::BOSS_CRAWL] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossCrawl"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::BOSS_SLEEP] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossSleep"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::BOSS_DEAD] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossDead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::BOSS_METEORREADY] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossChanneling"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

#pragma endregion 텍스쳐 컴포넌트 

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pStateMachine = dynamic_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CSkeletonKing::Change_Phase()
{
	if (10.f >= m_pBasicStat->Get_Stat()->fHP)
		m_ePhase = BOSSPHASE::LASTPHASE;
	else if((10.f < m_pBasicStat->Get_Stat()->fHP)&&(55.f >= m_pBasicStat->Get_Stat()->fHP))
		m_ePhase = BOSSPHASE::PHASE3;
	else if ((55.f < m_pBasicStat->Get_Stat()->fHP) && (70.f >= m_pBasicStat->Get_Stat()->fHP))
		m_ePhase = BOSSPHASE::PHASE2;
	else if (70.f < m_pBasicStat->Get_Stat()->fHP)
		m_ePhase = BOSSPHASE::PHASE1;
}

void CSkeletonKing::Set_Dir(_vec3 _vDir)
{
	m_vTargetPos = _vDir;
	m_vDir = m_vTargetPos - m_pTransform->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_fDelay = 0.f;
	m_bMove = true;
}

void CSkeletonKing::MoveToDir(const _float& fTimeDelta)
{
	if (2.f < m_fDelay)
	{
		m_fMoveDelay += fTimeDelta;
		m_pTransform->Translate(m_vDir);
		if (1.f < m_fMoveDelay)
		{
			m_pTransform->m_vInfo[INFO_POS] = m_vTargetPos;
			m_fDelay = 0.f;
			m_fMoveDelay = 0.f;
			m_bMove = false;
		}
	}
}

CSkeletonKing* CSkeletonKing::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkeletonKing* pInstance = new CSkeletonKing(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SkeletonKing Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkeletonKing::Free()
{
	__super::Free();
}
