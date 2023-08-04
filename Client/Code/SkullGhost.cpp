#include "..\Header\SkullGhost.h"
#include "Export_Function.h"
#include "Monster_Move.h"
#include "Ghost_IDLE.h"
#include "Ghost_Attack.h"
#include "Charge.h"
#include "Player.h"
#include "Monster_Fly.h"
#include "Warrior_Attack.h"
#include "EffectBlood.h"

#include "PoolManager.h"

CSkullGhost::CSkullGhost(LPDIRECT3DDEVICE9 pGrapicDev)
	: CMonster(pGrapicDev)
{
}

CSkullGhost::CSkullGhost(const CSkullGhost& rhs)
	: CMonster(rhs)
{
}

CSkullGhost::~CSkullGhost()
{
}

HRESULT CSkullGhost::Ready_Object()
{
	Set_ObjectTag(OBJECTTAG::MONSTER);
	Set_MonsterState(MONSTERTAG::SKULLGHOST);
	m_fSpeed = 30.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	CState* pState = CMonster_Move::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ROMIMG, pState);
	pState = CGhost_Attack::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ATTACK, pState);

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ROMIMG], STATE::ROMIMG, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::ROMIMG, pAnimation);
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ATTACK], STATE::ATTACK, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::ATTACK, pAnimation);

	Set_CenterPos(_vec3(-70.f, 0.f, -40.f));

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::ROMIMG);

	m_pTransform->Scale(_vec3(1.f, 1.f, 1.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(-10.f, 3.f, 10.f));

#pragma region SKULLGHOST
	Init_Stat();
#pragma endregion

	return S_OK;
}

_int CSkullGhost::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this); // ! 렌더를 맨 위에 올려 렌더는 되도록 하고


	if (SceneManager()->Get_GameStop()) { return 0; } // ! Esc 및 M키 누를 시 업데이트 멈추게 하는 용도 입니다.

	_int iExit = __super::Update_Object(fTimeDelta);

	if (IsKnockBack())
	{
		m_pStateMachine->Set_State(STATE::HIT);
		Set_KnockBack(false);
	}


	if (m_pBasicStat->Get_Stat()->fHealth <= 0)
	{
		m_pStateMachine->Set_State(STATE::DEAD);

		//////////////////////////////////////////////////////////////////////////////// 이펙트 
		if (!m_bDieEffect)
		{
			CGameObject* pGameObject = CEffectBlood::Create(m_pGraphicDev);
			pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y - .95f, m_pTransform->m_vInfo[INFO_POS].z));
			static_cast<CTempEffect*>(pGameObject)->Set_EffectColor(ECOLOR_RED);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

			m_bDieEffect = true;
		}
		//////////////////////////////////////////////////////////////////////////////// 이펙트 

	}

	m_pStateMachine->Update_StateMachine(fTimeDelta);


	return iExit;
}

void CSkullGhost::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; } // ! Esc 및 M키 누를 시 업데이트 멈추게 하는 용도 입니다.

	__super::LateUpdate_Object();
	//m_pTransform->Scale(_vec3(5.f, 5.f, 5.f));
}

void CSkullGhost::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();
}

void CSkullGhost::Init_Stat()
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

void CSkullGhost::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }


	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER
		&& this->Get_StateMachine()->Get_State() != STATE::DEAD &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM)
		__super::OnCollisionEnter(_pOther);

	// 충돌 밀어내기 후 이벤트 : 구현하시면 됩니다.

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

void CSkullGhost::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	// 충돌 밀어내기 후 이벤트 : 구현하시면 됩니다.
	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER &&
		this->Get_StateMachine()->Get_State() != STATE::DEAD &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM)
		__super::OnCollisionStay(_pOther);


}

void CSkullGhost::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::OnCollisionExit(_pOther);
}

HRESULT CSkullGhost::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);


	// Monster Animation Texture 
	//pComponent = m_pTexture[(_uint)STATE::IDLE] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SKULLGHOST"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::ROMIMG] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkullGhost"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::ATTACK] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkullGhostAttack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::HIT] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkullGhostHit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::DEAD] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkullGhostDead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);


	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pRigidBody = static_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	pComponent = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pStateMachine = static_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pBasicStat = static_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pAnimator = static_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);


	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CSkullGhost* CSkullGhost::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkullGhost* pInstance = new CSkullGhost(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CSkullGhost*>(pInstance);

		MSG_BOX("Create Ghost Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkullGhost::Free()
{
	__super::Free();
}
