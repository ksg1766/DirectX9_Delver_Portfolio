#include "stdafx.h"
#include "..\Header\Skeleton.h"
#include "Export_Function.h"
#include "Monster_Move.h"
#include "Warrior_Attack.h"
#include "Monster_Hit.h"
#include "Monster_Dead.h"
#include "Player.h"
#include "EffectBlood.h"

#include "SoundManager.h"
#include "PoolManager.h"

CSkeleton::CSkeleton(LPDIRECT3DDEVICE9 pGrapicDev)
	: Engine::CMonster(pGrapicDev), m_fFrame(0.f), m_bAttackTick(false)
{
}

CSkeleton::CSkeleton(const CSkeleton& rhs)
	: Engine::CMonster(rhs), m_fFrame(rhs.m_fFrame), m_bAttackTick(false)
{
}

CSkeleton::~CSkeleton()
{

}

HRESULT CSkeleton::Ready_Object()
{
	Set_ObjectTag(OBJECTTAG::MONSTER);
	Set_MonsterState(MONSTERTAG::SKELETON);
	m_bBlockOn = false;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	CState* pState = CMonster_Move::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ROMIMG, pState);
	pState = CWarror_Attack::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ATTACK, pState);
	pState = CMonster_Hit::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::HIT, pState);
	pState = CMonster_Dead::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::DEAD, pState);

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ROMIMG], STATE::ROMIMG, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::ROMIMG, pAnimation);
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ATTACK], STATE::ATTACK, 3.8f, TRUE);
	m_pAnimator->Add_Animation(STATE::ATTACK, pAnimation);
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::HIT], STATE::HIT, 3.f, TRUE);
	m_pAnimator->Add_Animation(STATE::HIT, pAnimation);
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::DEAD], STATE::DEAD, 3.f, TRUE);
	m_pAnimator->Add_Animation(STATE::DEAD, pAnimation);

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::ROMIMG);


	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	m_pTransform->Translate(_vec3(-10.f, 3.f, 10.f));

#pragma region Skeleton

	Init_Stat();

#pragma endregion

	return S_OK;
}

_int CSkeleton::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this); // ! 렌더를 맨 위에 올려 렌더는 되도록 하고

	if (SceneManager()->Get_GameStop()) { return 0; } // ! Esc 및 M키 누를 시 업데이트 멈추게 하는 용도 입니다.

	_int iExit = __super::Update_Object(fTimeDelta);

	if (IsKnockBack())
	{
		m_pStateMachine->Set_State(STATE::HIT);
		Set_KnockBack(false);
	}

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	_float fDistance = D3DXVec3Length(&(rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS]));

	if (fDistance < 15.f)
	{
		if (!m_bSearch)
		{
			m_bSearch = true;
			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SKELETON);
			CSoundManager::GetInstance()->PlaySound(L"en_slime_alert_02.mp3", CHANNELID::SOUND_SKELETON, 1.f);
		}
	}


	if (m_pBasicStat->Get_Stat()->fHP <= 0)
	{
		m_pStateMachine->Set_State(STATE::DEAD);
		//////////////////////////////////////////////////////////////////////////////// 이펙트 
	    if (!m_bDieEffect)
		{
			CGameObject* pGameObject = CEffectBlood::Create(m_pGraphicDev);
			pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y - .95f, m_pTransform->m_vInfo[INFO_POS].z));
			dynamic_cast<CTempEffect*>(pGameObject)->Set_EffectColor(ECOLOR_RED);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

			m_bDieEffect = true;
			rPlayer.Add_Exp(this);
		}
		//////////////////////////////////////////////////////////////////////////////// 이펙트 
		
		m_fDeadCoolTime += fTimeDelta;

		if (m_fDeadCoolTime > 3.f)
			CPoolManager::GetInstance()->Delete_Object(this);
	}

	m_pStateMachine->Update_StateMachine(fTimeDelta);

	//if (m_pStateMachine->Get_State() != STATE::ATTACK)
	//	ForceHeight(m_pTransform->m_vInfo[INFO_POS]);

	if (!Get_BlockOn())
		//ForceHeight(m_pTransform->m_vInfo[INFO_POS]);

	return iExit;
}

void CSkeleton::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; } // ! Esc 및 M키 누를 시 업데이트 멈추게 하는 용도 입니다.

	__super::LateUpdate_Object();
	//__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CSkeleton::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CSkeleton::Init_Stat()
{
	m_pBasicStat->Get_Stat()->fMaxHP = 4.f;
	m_pBasicStat->Get_Stat()->fHP = 4.f;
	m_pBasicStat->Get_Stat()->iDamageMin = 1;
	m_pBasicStat->Get_Stat()->iDamageMax = 2;
	m_pBasicStat->Get_Stat()->fSpeed = 4.f;
	m_pBasicStat->Get_Stat()->fAgility = 4.f;
	m_pBasicStat->Get_Stat()->fDeffense = 4.f;
	m_pBasicStat->Get_Stat()->fMagic = 4.f;
	m_pBasicStat->Get_Stat()->fAttack = 4.f;
	m_pBasicStat->Get_Stat()->iExp = 6.f;
}

void CSkeleton::OnCollisionEnter(CCollider* _pOther)
{
	// 충돌 밀어내기 후 이벤트 : 구현하시면 됩니다.
	if (SceneManager()->Get_GameStop()) { return ; }

	if (this->Get_StateMachine()->Get_State() != STATE::DEAD &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER)
		__super::OnCollisionEnter(_pOther);

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER
		&& this->Get_StateMachine()->Get_State() == STATE::ATTACK)
		if (!this->Get_AttackTick())
		{
			CPlayerStat& PlayerStat = *static_cast<CPlayer*>(_pOther->Get_Host())->Get_Stat();
			this->Set_AttackTick(true);
			IsAttack(&PlayerStat);

			//cout << "스켈레톤 공격" << endl;
		}
}

void CSkeleton::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (this->Get_StateMachine()->Get_State() != STATE::DEAD &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER)
	__super::OnCollisionStay(_pOther);
	// 충돌 밀어내기 후 이벤트 : 구현하시면 됩니다.

}

void CSkeleton::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::OnCollisionExit(_pOther);
	Set_BlockOn(false);
}

HRESULT CSkeleton::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);


	// Monster Animation Texture 
	pComponent = m_pTexture[(_uint)STATE::ROMIMG] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Skeleton"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::ATTACK] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkeletonAttack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::HIT] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkeletonHit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::DEAD] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkeletonDead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);


	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

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


CSkeleton* CSkeleton::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkeleton* pInstance = new CSkeleton(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CSkeleton*>(pInstance);

		MSG_BOX("Spiedt Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkeleton::Free()
{
	__super::Free();
}
