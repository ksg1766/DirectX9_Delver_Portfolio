#include "..\Header\DungeonWarrior.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "Warrior_Attack.h"
#include "Monster_Move.h"
#include "Monster_Hit.h"
#include "Monster_Dead.h"
#include "Longitudinal.h"
#include "HorizontalMove.h"
#include "Player.h"

CDungeonWarrior::CDungeonWarrior(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{
}

CDungeonWarrior::CDungeonWarrior(const CDungeonWarrior& rhs)
	: Engine::CMonster(rhs)
{

}

CDungeonWarrior::~CDungeonWarrior()
{

}

HRESULT CDungeonWarrior::Ready_Object()
{
	Set_ObjectTag(OBJECTTAG::MONSTER);
	Set_MonsterState(MONSTERTAG::WARRIOR);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(1.f, 0.f, 3.f));

	CState* pState = CHorizontalMove::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ROMIMG, pState);
	pState = CWarror_Attack::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ATTACK, pState);
	//pState = CMonster_Hit::Create(m_pGraphicDev, m_pStateMachine);
	//m_pStateMachine->Add_State(STATE::HIT, pState);
	//pState = CMonster_Dead::Create(m_pGraphicDev, m_pStateMachine);
	//m_pStateMachine->Add_State(STATE::DEAD, pState);


	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ROMIMG], STATE::ROMIMG, 5.f, TRUE);
	m_pAnimator->Add_Animation(STATE::ROMIMG, pAnimation);
	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ATTACK], STATE::ATTACK, 6.f, TRUE);
	m_pAnimator->Add_Animation(STATE::ATTACK, pAnimation);
	//pAnimation = CAnimation::Create(m_pGraphicDev,
	//	m_pTexture[(_uint)STATE::HIT], STATE::HIT, 5.f, TRUE);
	//m_pAnimator->Add_Animation(STATE::HIT, pAnimation);
	//pAnimation = CAnimation::Create(m_pGraphicDev,
	//	m_pTexture[(_uint)STATE::DEAD], STATE::DEAD, 3.f, TRUE);
	//m_pAnimator->Add_Animation(STATE::DEAD, pAnimation);

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::ROMIMG);

	m_pBasicStat->Get_Stat()->fAttack = 1.f;
	m_pBasicStat->Get_Stat()->fHealth = 5.f;


	return S_OK;
}

_int CDungeonWarrior::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);


	if (m_pBasicStat->Get_Stat()->fHealth <= 0)
	{
		if (m_pAnimator->Get_Animation()->Get_Frame() >= 1)
			m_pAnimator->Get_Animation()->Set_Loop(FALSE);

		m_pStateMachine->Set_State(STATE::DEAD);
	}

	m_pStateMachine->Update_StateMachine(fTimeDelta);
	//ForceHeight(m_pTransform->m_vInfo[INFO_POS]);

	return iExit;
}

void CDungeonWarrior::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();

	m_pStateMachine->LateUpdate_StateMachine();

	//__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CDungeonWarrior::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CDungeonWarrior::ForceHeight(_vec3 _vPos)
{
	_float x = (VTXCNTX * VTXITV / 2.f) + _vPos.x;
	_float z = (VTXCNTZ * VTXITV / 2.f) + _vPos.z;

	x /= (_float)VTXITV;
	z /= (_float)VTXITV;

	_int col = ::floorf(x);
	_int row = ::floorf(z);

	_vec3 A = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col];
	_vec3 B = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col + 1];
	_vec3 C = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col];
	_vec3 D = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col + 1];

	_float dx = x - col;
	_float dz = z - row;

	_float height;
	//c-d b-d cdb 
	if (dz < 1.0f - dx)
	{
		/*
		Lerp(_float _a, _float _b, _float _c)
		{
			return a - (a * t) + (b * t);
		}
		*/

		_vec3 uy = B - A;
		_vec3 vy = C - A;

		height = A.y + (uy.y * dx) + (vy.y * dz) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height;
	}// c-a b-a cba
	else
	{
		_vec3 uy = C - D;
		_vec3 vy = B - D;

		height = D.y + (uy.y * (1.f - dx)) + (vy.y * (1.f - dz)) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height;
	}
}

void CDungeonWarrior::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }


	if (this->Get_StateMachine()->Get_State() != STATE::DEAD && 
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM)
		__super::OnCollisionEnter(_pOther);

	// 충돌 밀어내기 후 이벤트 : 구현하시면 됩니다.

	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER
		&& this->Get_StateMachine()->Get_State() == STATE::ATTACK)
	{
		CPlayerStat& PlayerState = *static_cast<CPlayer*>(_pOther->GetHost())->Get_Stat();

		if (!this->Get_AttackTick())
		{
			PlayerState.Take_Damage(this->Get_BasicStat()->Get_Stat()->fAttack);
			this->Set_AttackTick(true);

			cout << "워리어 공격" << endl;
		}

	}
}

void CDungeonWarrior::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	// 충돌 밀어내기 후 이벤트 : 구현하시면 됩니다.
}

void CDungeonWarrior::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}


HRESULT CDungeonWarrior::Add_Component()
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

	pComponent = m_pTexture[(_uint)STATE::ROMIMG] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Warrior"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::ATTACK] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WarriorAttack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::HIT] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WarriorHit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
	pComponent = m_pTexture[(_uint)STATE::DEAD] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_WarriorDead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pAnimator = static_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	pComponent = m_pStateMachine = static_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pBasicStat = static_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CDungeonWarrior* CDungeonWarrior::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDungeonWarrior* pInstance = new CDungeonWarrior(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CDungeonWarrior*>(pInstance);

		MSG_BOX("Warrior Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDungeonWarrior::Free()
{
	__super::Free();
}
