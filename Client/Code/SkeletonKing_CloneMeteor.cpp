#include "stdafx.h"
#include "SkeletonKing_CloneMeteor.h"
#include "Export_Function.h"
#include "BossProjectile.h"
#include "BossExplosion.h"
#include "SkeletonKing.h"
#include "Boss_CloneMeteorState.h"
#include "Player.h"
#include "Boss_CloneMeteorDead.h"

CSkeletonKing_CloneMeteor::CSkeletonKing_CloneMeteor(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{

}

CSkeletonKing_CloneMeteor::CSkeletonKing_CloneMeteor(const CSkeletonKing_CloneMeteor& rhs)
	: Engine::CMonster(rhs)
{
}

CSkeletonKing_CloneMeteor::~CSkeletonKing_CloneMeteor()
{
	Free();
}

HRESULT CSkeletonKing_CloneMeteor::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BOSS;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));
	m_fDelay = 0.f;
	m_fDeleteDelay = 0.f;
	m_fMoveDelay = 0.f;
	m_bHit = false;
	m_bMove = false;
	m_bMeteor = false;
	m_pBasicStat->Get_Stat()->fMaxHP = 4.f;
	m_pBasicStat->Get_Stat()->fHP = 4.f;
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	
	CState* m_pState = CBoss_CloneMeteorState::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, m_pState);
	m_pState = CBoss_CloneMeteorDead::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::DEAD, m_pState);

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::IDLE], STATE::IDLE, 12.f, TRUE);
	m_pAnimator->Add_Animation(STATE::IDLE, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::DEAD], STATE::DEAD, 12.f, FALSE);
	m_pAnimator->Add_Animation(STATE::DEAD, pAnimation);

	m_pStateMachine->Set_State(STATE::IDLE);
	m_pAnimator->Set_Animation(STATE::IDLE);
	return S_OK;
}

_int CSkeletonKing_CloneMeteor::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);
	m_pStateMachine->Update_StateMachine(fTimeDelta);
	m_fDelay += fTimeDelta;
	if ((2.f < m_fDelay)&&(!m_bMove))
	{
		m_fMoveDelay += fTimeDelta;
		MoveToDir();
	}

	if (STATE::BOSS_STURN == static_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_StateMachine()->Get_State())
	{
		m_pStateMachine->Set_State(STATE::DEAD);
	}
	
	if (0 >= m_pBasicStat->Get_Stat()->fHP)
	{
		m_pStateMachine->Set_State(STATE::DEAD);
	}
	if ((BOSSPHASE::PHASE2 != static_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Phase()))
	{
		static_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Add_CloneCount(-1);
		m_bMove = false;
		m_pStateMachine->Set_State(STATE::DEAD);
	}

	if (STATE::DEAD == m_pStateMachine->Get_State())
	{
		m_fDeleteDelay += fTimeDelta;
		if (2.f < m_fDeleteDelay)
		{
			m_fDeleteDelay = 0.f;
			static_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Add_CloneCount(-1);

			Engine::CGameObject* pGameObject = nullptr;
			pGameObject = CBossExplosion::Create(m_pGraphicDev);
			static_cast<CBossExplosion*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_pTransform->m_vInfo[INFO_POS]);
			static_cast<CBossExplosion*>(pGameObject)->Set_Scale(3.f);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			m_bMove = false;
			m_IsDead = true;
		}
	}

	if ((m_bHit) && (1.f < m_fDelay))
	{
		m_bMove = false;
		m_fDelay = 0.f;
		m_bHit = false;
	}
	return iExit;
}

void CSkeletonKing_CloneMeteor::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
    if(STATE::BOSS_PH2SKILL5 != static_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_StateMachine()->Get_State())
		m_pStateMachine->Set_State(STATE::DEAD);
	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));
}

void CSkeletonKing_CloneMeteor::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pBuffer->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CSkeletonKing_CloneMeteor::MoveToDir()
{
	m_pTransform->Translate(m_vDir);
	if (1.f < m_fMoveDelay)
	{
		m_pTransform->m_vInfo[INFO_POS] = m_vTargetPos;
		m_bMove = true;
		m_fDelay = 0.f;
		m_fMoveDelay = 0.f;
	}
}

void CSkeletonKing_CloneMeteor::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CSkeletonKing_CloneMeteor::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (m_bHit) { return; }
	if (OBJECTTAG::PLAYERBULLET == _pOther->Get_ObjectTag()||(OBJECTTAG::ITEM == _pOther->Get_ObjectTag()))
	{
		if (OBJECTTAG::ITEM == _pOther->Get_ObjectTag())
			if (!static_cast<CPlayer*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front())->Get_Attack())
				return;
			m_bHit = true;
	}
}

void CSkeletonKing_CloneMeteor::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CSkeletonKing_CloneMeteor::Set_Dir(_vec3 _vDir)
{
	m_vTargetPos = _vDir;
	//m_vDir = m_vTargetPos - m_pTransform->m_vInfo[INFO_POS];
	m_vDir = m_vTargetPos - _vec3(-72.5f, 38.f, 94.5f);
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}

HRESULT CSkeletonKing_CloneMeteor::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pTexture[(_uint)STATE::IDLE] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Boss"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::DEAD] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossDying"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = static_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pStateMachine = static_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pAnimator = static_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CSkeletonKing_CloneMeteor* CSkeletonKing_CloneMeteor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkeletonKing_CloneMeteor* pInstance = new CSkeletonKing_CloneMeteor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SkeletonKing Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CSkeletonKing_CloneMeteor::Free()
{
	__super::Free();
}

void CSkeletonKing_CloneMeteor::Init_Stat()
{
}