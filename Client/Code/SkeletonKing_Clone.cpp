#include "stdafx.h"
#include "SkeletonKing_Clone.h"
#include "Export_Function.h"
#include "BossProjectile.h"
#include "BossExplosion.h"
#include "SkeletonKing.h"
#include "Boss_CloneState.h"
#include "Player.h"
#include "Boss_CloneDead.h"
CSkeletonKing_Clone::CSkeletonKing_Clone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{

}

CSkeletonKing_Clone::CSkeletonKing_Clone(const CSkeletonKing_Clone& rhs)
	: Engine::CMonster(rhs)
{
}

CSkeletonKing_Clone::~CSkeletonKing_Clone()
{
	Free();
}

HRESULT CSkeletonKing_Clone::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BOSS;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));
	m_fDelay = 0.f;
	m_fDeleteDelay = 0.f;
	m_fMoveDelay = 0.f;
	m_bHit = false;
	m_bMove = false;
	m_pBasicStat->Get_Stat()->fHP = 4.f;
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	
	CState* m_pState = CBoss_CloneState::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, m_pState);
	m_pState = CBoss_CloneDead::Create(m_pGraphicDev, m_pStateMachine);
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

_int CSkeletonKing_Clone::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);
	m_fDelay += fTimeDelta;
	m_pStateMachine->Update_StateMachine(fTimeDelta);
	if ((2.f < m_fDelay)&&(!m_bMove))
	{
		m_fMoveDelay += fTimeDelta;
		MoveToDir();
	}
	if (STATE::BOSS_STURN == dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_StateMachine()->Get_State())
	{
		m_pStateMachine->Set_State(STATE::DEAD);
	}
	else if (0 >= m_pBasicStat->Get_Stat()->fHP)
	{
		m_pStateMachine->Set_State(STATE::DEAD);
	}
	else if ((0 >= dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_CloneCount()) || (BOSSPHASE::PHASE2 != dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Phase()))
	{
		dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Add_CloneCount(-1);
		m_bMove = false;
		m_IsDead = true;
	}
	if ((m_bHit)&&(1.f < m_fDelay))
	{
		m_bMove = false;
		m_fDelay = 0.f;
		m_bHit = false;
	}
	if (STATE::DEAD == m_pStateMachine->Get_State())
	{
		m_fDeleteDelay += fTimeDelta;
		if (4.f < m_fDeleteDelay)
		{
			m_fDeleteDelay = 0.f;
			dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Add_CloneCount(-1);

			Engine::CGameObject* pGameObject = nullptr;
			pGameObject = CBossExplosion::Create(m_pGraphicDev);
			dynamic_cast<CBossExplosion*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_pTransform->m_vInfo[INFO_POS]);
			dynamic_cast<CBossExplosion*>(pGameObject)->Set_Scale(3.f);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			m_bMove = false;
			m_IsDead = true;
		}
	}
	return iExit;
}

void CSkeletonKing_Clone::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));
}

void CSkeletonKing_Clone::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	if(STATE::DEAD == m_pStateMachine->Get_State())
		m_pStateMachine->Ready_StateMachine();
	m_pBuffer->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CSkeletonKing_Clone::MoveToDir()
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

void CSkeletonKing_Clone::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CSkeletonKing_Clone::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (m_bHit) { return; }
	if (OBJECTTAG::PLAYERBULLET == _pOther->Get_ObjectTag()||(OBJECTTAG::ITEM == _pOther->Get_ObjectTag()))
	{
		if (OBJECTTAG::ITEM == _pOther->Get_ObjectTag())
			if (!dynamic_cast<CPlayer*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front())->Get_Attack())
				return;
			m_bHit = true;
	}
}

void CSkeletonKing_Clone::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CSkeletonKing_Clone::Set_Dir(_vec3 _vDir)
{
	m_vTargetPos = _vDir;
	m_vDir = m_vTargetPos - m_pTransform->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}

HRESULT CSkeletonKing_Clone::Add_Component(void)
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

	pComponent = m_pTexture[(_uint)STATE::IDLE] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Boss"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTexture[(_uint)STATE::DEAD] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossDying"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pStateMachine = dynamic_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CSkeletonKing_Clone* CSkeletonKing_Clone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkeletonKing_Clone* pInstance = new CSkeletonKing_Clone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SkeletonKing Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkeletonKing_Clone::Free()
{
	__super::Free();
}

void CSkeletonKing_Clone::Init_Stat()
{
}