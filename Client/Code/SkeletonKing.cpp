#include "..\Header\SkeletonKing.h"
#include "Export_Function.h"
#include "Boss_Sleep.h"
#include "Boss_WakeUp.h"
#include "Boss_Idle.h"
#include "Boss_Attack.h"
#include "FirePattern.h"
#include "ExplosionPattern.h"
#include "TeleportPattern.h"
#include "Boss_SkeletonSpawnPattern.h"
#include "CrawlPattern.h"
#include "Boss_Sturn.h"
#include "Boss_MeteorReady.h"
#include "Boss_Dying.h"
#include "Boss_Dead.h"
#include "FireWallPttern.h"
#include "FireWavePattern.h"
#include "Clone_Pattern.h"
#include "LostSoulPattern.h"
#include "GrapPattern.h"
#include "CrossPattern.h"
#include "MeteorPh2.h"
#include "FIreWave2.h"
#include "LostSoulPattern2.h"
#include "LightningPattern2.h"
#include "MeteorPh3.h"
#include "Player.h"
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
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));
	m_pBasicStat->Get_Stat()->fHealth = 100.f;
	m_iHitCount = 0;
	m_fHitCool = 0.f;
	m_b3phase = false;
	m_bSturn = false;
	m_iCloneCount = 0;
#pragma region 상태

	//상태추가
	CState* m_pState = CBoss_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_IDLE, m_pState);

	m_pState = CBoss_Sleep::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_SLEEP, m_pState);

	m_pState = CBoss_WakeUp::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_WAKEUP, m_pState);

	m_pState = CBoss_Attack::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_ATTACK, m_pState);
	
	m_pState = CFirePattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL1, m_pState);

	m_pState = CExplosionPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL2, m_pState);

	m_pState = CBoss_SkeletonSpawnPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL3, m_pState);

	m_pState = CFireWallPttern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL4, m_pState);

	m_pState = CBoss_MeteorReady::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH1SKILL5, m_pState);

	m_pState = CClone_Pattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH2SKILL1, m_pState);

	m_pState = CCrossPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH2SKILL2, m_pState);

	m_pState = CFireWavePattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH2SKILL3, m_pState);

	//m_pState = CGrapPattern::Create(m_pGraphicDev, m_pStateMachine);
	//m_pStateMachine->Add_State(STATE::BOSS_PH2SKILL4, m_pState);//일단 보류

	m_pState = CLostSoulPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH2SKILL4, m_pState);

	m_pState = CBoss_Meteor2Ph::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH2SKILL5, m_pState);

	m_pState = CFIreWave2::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH3SKILL1, m_pState);

	m_pState = CLostSoulPattern2::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH3SKILL2, m_pState);

	m_pState = CLightningPattern2::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH3SKILL3, m_pState);

	m_pState = CMeteorPh3::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_PH3SKILL5, m_pState);

	m_pState = CCrawlPattern::Create(m_pGraphicDev, m_pStateMachine);
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
		m_pTexture[(_uint)STATE::BOSS_IDLE], STATE::BOSS_IDLE, 8.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_IDLE, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_SLEEP], STATE::BOSS_SLEEP, 8.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_SLEEP, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_WAKEUP], STATE::BOSS_WAKEUP, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_WAKEUP, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_IDLE], STATE::BOSS_ATTACK, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_ATTACK, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH1SKILL1, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL1, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH1SKILL2, 10.f, FALSE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL2, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH1SKILL3, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL3, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_IDLE], STATE::BOSS_PH1SKILL4, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL4, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH1SKILL5, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH1SKILL5, pAnimation);
	
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH2SKILL1, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH2SKILL1, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH2SKILL2, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH2SKILL2, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH2SKILL3, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH2SKILL3, pAnimation);
	
	/*pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH2SKILL4, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH2SKILL4, pAnimation);*/

		pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH2SKILL4, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH2SKILL4, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH2SKILL5, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH2SKILL5, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH3SKILL1, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH3SKILL1, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_PH3SKILL2, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH3SKILL2, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH3SKILL3, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH3SKILL3, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_METEORREADY], STATE::BOSS_PH3SKILL5, 15.f, TRUE);
	m_pAnimator->Add_Animation(STATE::BOSS_PH3SKILL5, pAnimation);
	
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

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::BOSS_SLEEP);
	return S_OK;
}

_int CSkeletonKing::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);
	m_pStateMachine->Update_StateMachine(fTimeDelta);
	Key_Input();
	return iExit;
}

void CSkeletonKing::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
	//if(!m_b3phase)
	//	m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));
	//if(m_b3phase)
	//	m_pTransform->Scale(_vec3(12.f, 12.f, 12.f));
	m_pTransform->Scale(_vec3(12.f, 12.f, 12.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale()*0.8f);
}

void CSkeletonKing::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	/*if (STATE::BOSS_TELEPORT != m_pStateMachine->Get_State())
	{
		m_pStateMachine->Render_StateMachine();
		m_pBuffer->Render_Buffer();
	}*/
	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();
#if _DEBUG
	m_pCollider->Render_Collider();
#endif

}

void CSkeletonKing::Init_Stat()
{
	/*m_pBasicStat->Get_Stat()->fMaxHP = 4.f;
	m_pBasicStat->Get_Stat()->fHP = 4.f;
	m_pBasicStat->Get_Stat()->iDamageMin = 1;
	m_pBasicStat->Get_Stat()->iDamageMax = 2;
	m_pBasicStat->Get_Stat()->fSpeed = 4.f;
	m_pBasicStat->Get_Stat()->fAgility = 4.f;
	m_pBasicStat->Get_Stat()->fDeffense = 4.f;
	m_pBasicStat->Get_Stat()->fMagic = 4.f;
	m_pBasicStat->Get_Stat()->fAttack = 4.f;*/
}

void CSkeletonKing::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::OnCollisionEnter(_pOther);
}

void CSkeletonKing::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

}

void CSkeletonKing::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::OnCollisionExit(_pOther);
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
	if (3 <= m_iHitCount)
	{
		m_iHitCount = 0.f;
		m_pStateMachine->Set_State(STATE::BOSS_STURN);
		//m_bSturn = true;
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

	/*pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);*/

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

void CSkeletonKing::Key_Input()
{
	if (Engine::InputDev()->Key_Down(DIK_J))
	{
		m_pStateMachine->Set_State(STATE::BOSS_PH3SKILL4);
	}
	if (Engine::InputDev()->Key_Down(DIK_K))
	{
		m_pStateMachine->Set_State(STATE::BOSS_PH2SKILL2);
	}
	//if (Engine::InputDev()->Key_Down(DIK_L))
	//{
	//	m_pStateMachine->Set_State(STATE::BOSS_CRAWL);
	//}
	//if (Engine::InputDev()->Key_Down(DIK_P))
	//{
	//	m_pStateMachine->Set_State(STATE::BOSS_IDLE);
	//}

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
