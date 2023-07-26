#include "Blade_Trap_Blade.h"
#include "Export_Function.h"
#include "Blade_Trap_Idle.h"
#include "Blade_Trap_Attack.h"
#include "Player.h"
CBlade_Trap_Blade::CBlade_Trap_Blade(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CTrap(pGraphicDev)
{
}

CBlade_Trap_Blade::CBlade_Trap_Blade(const CBlade_Trap_Blade& rhs)
	: Engine::CTrap(rhs)
{
}

CBlade_Trap_Blade::~CBlade_Trap_Blade()
{
}

HRESULT CBlade_Trap_Blade::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::TRAP;
	m_eTrapTag = TRAPTAG::TRAP_END;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	CState* pState = CBlade_Trap_Idle::Create(m_pGraphicDev, m_pStateMachine);
	dynamic_cast<CBlade_Trap_Idle*>(pState)->Set_TrapCenter(m_vTrapCenter);
	m_pStateMachine->Add_State(STATE::IDLE, pState);

	pState = CBlade_Trap_Attack::Create(m_pGraphicDev, m_pStateMachine);
	dynamic_cast<CBlade_Trap_Attack*>(pState)->Set_TrapHeight(m_vTrapCenter.y);
	m_pStateMachine->Add_State(STATE::ATTACK, pState);
	
	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
	m_pTexture[(_uint)STATE::IDLE], STATE::IDLE, 8.f, FALSE);
	m_pAnimator->Add_Animation(STATE::IDLE, pAnimation);

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::IDLE);

	m_pTransform->Scale(_vec3(0.7f, 1.f, 0.7f));
	m_bHit = false;
	m_bCollider = false;
	m_fHitTime = 0.f;

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale() * 1.0f);

	return S_OK;
}

_int CBlade_Trap_Blade::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_uint iExit = 0;
	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		return iExit;

	if (SceneManager()->Get_GameStop()) { return 0; }
	iExit = __super::Update_Object(fTimeDelta);

	//m_pStateMachine->Update_StateMachine(fTimeDelta);
	m_fHitTime += fTimeDelta;
	if ((m_bHit)&&(1.5f < m_fHitTime))
	{
		m_fHitTime = 0.f;
		m_bHit = false;
	}
	return iExit;
}

void CBlade_Trap_Blade::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CBlade_Trap_Blade::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

#if _DEBUG
		m_pCollider->Render_Collider();
#endif
}

HRESULT CBlade_Trap_Blade::Ready_Object2(_vec3 vCenterPos)
{
	m_eObjectTag = OBJECTTAG::MONSTER;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	CState* pState = CBlade_Trap_Idle::Create(m_pGraphicDev, m_pStateMachine);
	dynamic_cast<CBlade_Trap_Idle*>(pState)->Set_TrapCenter(vCenterPos);
	m_pStateMachine->Add_State(STATE::IDLE, pState);

	pState = CBlade_Trap_Attack::Create(m_pGraphicDev, m_pStateMachine);
	dynamic_cast<CBlade_Trap_Attack*>(pState)->Set_TrapHeight(vCenterPos.y);
	m_pStateMachine->Add_State(STATE::ATTACK, pState);

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::IDLE], STATE::IDLE, 8.f, FALSE);
	m_pAnimator->Add_Animation(STATE::IDLE, pAnimation);

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::IDLE);

	m_pTransform->Scale(_vec3(0.7f, 1.f, 0.7f));
	m_bHit = false;
	m_bCollider = false;
	m_fHitTime = 0.f;
	return S_OK;
}

void CBlade_Trap_Blade::Set_Collider()
{
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale()*0.5f);
	m_bCollider = true;
}

void CBlade_Trap_Blade::Set_TrapCenter(_vec3 _vCenter)
{ 
	m_vTrapCenter = _vCenter; 
}

void CBlade_Trap_Blade::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if ((!m_bCollider) || (m_bHit)) { return; }
	m_pOtherObj = _pOther->Get_Host();
	if (OBJECTTAG::PLAYER == m_pOtherObj->Get_ObjectTag())
	{
		CPlayerStat& PlayerState = *static_cast<CPlayer*>(_pOther->Get_Host())->Get_Stat();
		PlayerState.Take_Damage(4.f);
		m_bHit = true;
	}
}

void CBlade_Trap_Blade::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CBlade_Trap_Blade::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CBlade_Trap_Blade::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture[(_uint)STATE::IDLE] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BladeTrapBlade"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

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

CBlade_Trap_Blade* CBlade_Trap_Blade::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBlade_Trap_Blade* pInstance = new CBlade_Trap_Blade(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Blade_Trap_Blade Create Failed");
		return nullptr;
	}

	return pInstance;
}

CBlade_Trap_Blade* CBlade_Trap_Blade::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCenterPos)
{
	CBlade_Trap_Blade* pInstance = new CBlade_Trap_Blade(pGraphicDev);

	if (FAILED(pInstance->Ready_Object2(vCenterPos)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Blade_Trap_Blade Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBlade_Trap_Blade::Free()
{
	__super::Free();
}
