#include "stdafx.h"
#include "..\Header\SkeletonKing.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "Boss_Sleep.h"
#include "Boss_WakeUp.h"
#include "Boss_Idle.h"
#include "Boss_Attack.h"
#include "FirePattern.h"
#include "ExplosionPattern.h"
#include "TeleportPattern.h"
#include "CrawlPattern.h"
#include "SpawnPattern.h"
#include "Boss_Sturn.h"
#include "Boss_MeteorReady.h"
#include "Boss_Dying.h"
#include "Boss_Dead.h"

CSkeletonKing::CSkeletonKing(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSkeletonKing::CSkeletonKing(const CSkeletonKing& rhs)
	: Engine::CGameObject(rhs)
{
}

CSkeletonKing::~CSkeletonKing()
{
}

HRESULT CSkeletonKing::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BOSS;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(1.f, 1.f, 1.f));

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	m_pBasicStat->Get_Stat()->fHealth = 100.f;

	m_pTransform->Translate(_vec3(30.f, 0.f, 30.f));

#pragma region 상태

	//상태추가
	CState* pState = CBoss_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_IDLE, pState);

	pState = CBoss_Sleep::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_SLEEP, pState);

	pState = CBoss_WakeUp::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_WAKEUP, pState);

	pState = CBoss_Attack::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_ATTACK, pState);
	//
	pState = CFirePattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_FIRE, pState);

	pState = CExplosionPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_EXPLOSION, pState);

	pState = CTeleportPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_TELEPORT, pState);

	pState = CCrawlPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_CRAWL, pState);
	
	pState = CSpawnPattern::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_SPAWNMONSTER, pState);

	pState = CBoss_Sturn::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_STURN, pState);

	pState = CBoss_MeteorReady::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_METEORREADY, pState);

	pState = CBoss_Dying::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_DYING, pState);

	pState = CBoss_Dead::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::BOSS_DEAD, pState);

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
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_FIRE, 5.f, FALSE);
	m_pAnimator->Add_Animation(STATE::BOSS_FIRE, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_EXPLOSION, 5.f, FALSE);
	m_pAnimator->Add_Animation(STATE::BOSS_EXPLOSION, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_TELEPORT, 5.f, FALSE);
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
		m_pTexture[(_uint)STATE::BOSS_ATTACK], STATE::BOSS_SPAWNMONSTER, 15.f, FALSE);
	m_pAnimator->Add_Animation(STATE::BOSS_SPAWNMONSTER, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_DYING], STATE::BOSS_DYING, 6.f, FALSE);
	m_pAnimator->Add_Animation(STATE::BOSS_DYING, pAnimation);

	pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::BOSS_DEAD], STATE::BOSS_DEAD, 0.f, FALSE);
	m_pAnimator->Add_Animation(STATE::BOSS_DEAD, pAnimation);

#pragma endregion 애니메이션

	m_pStateMachine->Set_Animator(m_pAnimator);
	//m_pStateMachine->Set_State(STATE::BOSS_IDLE);
	m_pStateMachine->Set_State(STATE::BOSS_SLEEP);
	return S_OK;
}

_int CSkeletonKing::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	ForceHeight(m_pTransform->m_vInfo[INFO_POS]);
	m_pStateMachine->Update_StateMachine(fTimeDelta);
	return iExit;
}

void CSkeletonKing::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	m_pBillBoard->LateUpdate_Component();
	Key_Input();
	__super::LateUpdate_Object();
	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CSkeletonKing::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pStateMachine->Render_StateMachine();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CSkeletonKing::ForceHeight(_vec3 _vPos)
{
	if (STATE::BOSS_METEORREADY == m_pStateMachine->Get_State())
		return;
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

	if (dz < 1.0f - dx)
	{

		_vec3 uy = B - A;
		_vec3 vy = C - A;

		height = A.y + (uy.y * dx) + (vy.y * dz) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height;
	}
	else
	{
		_vec3 uy = C - D;
		_vec3 vy = B - D;

		height = D.y + (uy.y * (1.f - dx)) + (vy.y * (1.f - dz)) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height;
	}
}

void CSkeletonKing::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

#pragma region 밀어내기
	_vec3	vOtherPos = _pOther->GetCenterPos();
	_float* fOtherAxis = _pOther->GetAxisLen();

	_vec3	vThisPos = m_pCollider->GetCenterPos();
	_float* fThisAxis = m_pCollider->GetAxisLen();

	// OBJECTTAG에 따른 예외 처리 가능성
	_float fWidth = fabs(vOtherPos.x - vThisPos.x);
	_float fHeight = fabs(vOtherPos.y - vThisPos.y);
	_float fDepth = fabs(vOtherPos.z - vThisPos.z);

	_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
	_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
	_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

	_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

	if (fRadiusX == fMinAxis)
	{
		if (vOtherPos.x < vThisPos.x)
			m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
		else
			m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
	}
	else if (fRadiusZ == fMinAxis)
	{
		if (vOtherPos.z < vThisPos.z)
			m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
		else
			m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
	}
	else //(fRadiusY == fMinAxis)
	{
		if (vOtherPos.y < vThisPos.y)
			m_pTransform->Translate(_vec3(0.f, fRadiusY, 0.f));
		else
			m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
	}
#pragma endregion 밀어내기
}

void CSkeletonKing::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

#pragma region 밀어내기
	_vec3	vOtherPos = _pOther->GetCenterPos();
	_float* fOtherAxis = _pOther->GetAxisLen();

	_vec3	vThisPos = m_pCollider->GetCenterPos();
	_float* fThisAxis = m_pCollider->GetAxisLen();

	// OBJECTTAG에 따른 예외 처리 가능성
	_float fWidth = fabs(vOtherPos.x - vThisPos.x);
	_float fHeight = fabs(vOtherPos.y - vThisPos.y);
	_float fDepth = fabs(vOtherPos.z - vThisPos.z);

	_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
	_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
	_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

	_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

	if (fRadiusX == fMinAxis)
	{
		if (vOtherPos.x < vThisPos.x)
			m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
		else
			m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
	}
	else if (fRadiusZ == fMinAxis)
	{
		if (vOtherPos.z < vThisPos.z)
			m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
		else
			m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
	}
	else //(fRadiusY == fMinAxis)
	{
		if (vOtherPos.y < vThisPos.y)
			m_pTransform->Translate(_vec3(0.f, fRadiusY, 0.f));
		else
			m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
	}
#pragma endregion 밀어내기
}

void CSkeletonKing::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
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

	pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

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
	if (Engine::InputDev()->Key_Down(DIK_Z))
	{
		m_pStateMachine->Set_State(STATE::BOSS_EXPLOSION);
	}
	if (Engine::InputDev()->Key_Down(DIK_X))
	{
		m_pStateMachine->Set_State(STATE::BOSS_TELEPORT);
	}
	if (Engine::InputDev()->Key_Down(DIK_V))
	{
		m_pStateMachine->Set_State(STATE::BOSS_METEORREADY);
	}
	if (Engine::InputDev()->Key_Down(DIK_B))
	{
		m_pStateMachine->Set_State(STATE::BOSS_SPAWNMONSTER);
	}
	if (Engine::InputDev()->Key_Down(DIK_N))
	{
		m_pStateMachine->Set_State(STATE::BOSS_CRAWL);
	}
	if (Engine::InputDev()->Key_Down(DIK_L))
	{
		m_pStateMachine->Set_State(STATE::BOSS_STURN);
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
