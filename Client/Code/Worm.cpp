#include "Worm.h"
#include "Export_Function.h"
#include "Monster_Move.h"
#include "Monster_Jump.h"
#include "Monster_Hit.h"
#include "Monster_Dead.h"
#include "Player.h"

CWorm::CWorm(LPDIRECT3DDEVICE9 pGrapicDev)
	: Engine::CMonster(pGrapicDev), m_fFrame(0.f), m_bAttackTick(false)
{
}

CWorm::CWorm(const CWorm& rhs)
	: Engine::CMonster(rhs), m_fFrame(rhs.m_fFrame), m_bAttackTick(false)
{
}

CWorm::~CWorm()
{

}

HRESULT CWorm::Ready_Object()
{
	Set_ObjectTag(OBJECTTAG::MONSTER);
	Set_MonsterState(MONSTERTAG::WORM);
	m_bBlockOn = false;
	m_bWallTouch = false;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	CState* pState = CMonster_Move::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ROMIMG, pState);
	pState = CMonster_Jump::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ATTACK, pState);
	pState = CMonster_Hit::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::HIT, pState);
	pState = CMonster_Dead::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::DEAD, pState);


	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ROMIMG], STATE::ROMIMG, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::ROMIMG, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ATTACK], STATE::ATTACK, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::ATTACK, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::HIT], STATE::HIT, 3.f, TRUE);
	m_pAnimator->Add_Animation(STATE::HIT, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::DEAD], STATE::DEAD, 3.f, TRUE);
	m_pAnimator->Add_Animation(STATE::DEAD, pAnimation);

	m_pStateMachine->Set_Animator(m_pAnimator);

	m_pStateMachine->Set_State(STATE::ROMIMG);

	m_pBasicStat->Get_Stat()->fHealth = 10.f;

	m_pBasicStat->Get_Stat()->fAttack = 2.f;

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(2.f, 3.f, 5.f));

	return S_OK;
}

_int CWorm::Update_Object(const _float& fTimeDelta)
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
		m_pStateMachine->Set_State(STATE::DEAD);


	m_pStateMachine->Update_StateMachine(fTimeDelta);

	//if (m_pStateMachine->Get_State() != STATE::ATTACK)
		//ForceHeight(m_pTransform->m_vInfo[INFO_POS]);

	// ksg
	if (m_pStateMachine->Get_State() == STATE::ATTACK)
		m_pRigidBody->UseGravity(false);
	else
		m_pRigidBody->UseGravity(true);

	return iExit;
}

void CWorm::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; } // ! Esc 및 M키 누를 시 업데이트 멈추게 하는 용도 입니다.

	__super::LateUpdate_Object();

	//__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CWorm::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CWorm::Init_Stat()
{
}

void CWorm::OnCollisionEnter(CCollider* _pOther)
{
	// 충돌 밀어내기 후 이벤트 : 구현하시면 됩니다.
	if (SceneManager()->Get_GameStop()) { return; }

	if (this->Get_StateMachine()->Get_State() != STATE::DEAD &&
		_pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::ITEM &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER)
		__super::OnCollisionEnter(_pOther);

	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER
		&& this->Get_StateMachine()->Get_State() == STATE::ATTACK)
	{
		CPlayerStat& PlayerState = *dynamic_cast<CPlayer*>(_pOther->GetHost())->Get_Stat();

		if (!this->Get_AttackTick())
		{
			PlayerState.Take_Damage(this->Get_BasicStat()->Get_Stat()->fAttack);
			this->Set_AttackTick(true);

			cout << "거미 공격" << endl;
		}
	}

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BLOCK)
	{
		Set_WallTouch(true);
	}
}

void CWorm::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (this->Get_StateMachine()->Get_State() != STATE::DEAD &&
		_pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::ITEM &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER)
	__super::OnCollisionStay(_pOther);

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BLOCK)
	{
		Set_WallTouch(true);
	}

	// 충돌 밀어내기 후 이벤트 : 구현하시면 됩니다.
}

void CWorm::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::OnCollisionExit(_pOther);
	//Set_WallTouch(false);
}

HRESULT CWorm::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	// Monster Animation Texture 
	pComponent = m_pTexture[(_uint)STATE::ROMIMG] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Worm"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::ATTACK] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WormAttack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::HIT] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WormHit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::DEAD] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WormDead"));
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

CWorm* CWorm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWorm* pInstance = new CWorm(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CWorm*>(pInstance);

		MSG_BOX("Spiedt Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CWorm::Free()
{
	__super::Free();
}
