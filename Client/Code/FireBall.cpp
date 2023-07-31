#include "..\Header\FireBall.h"
#include "Export_Function.h"
#include "Player.h"
#include "EffectProjectileTrace.h"

#include "FireWands.h"

#include "EffectExplosion.h"
#include "EffectSquare.h"

CFireBall::CFireBall(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CFireBall::CFireBall(const CFireBall& rhs)
	: Engine::CGameObject(rhs)
{
}

CFireBall::~CFireBall()
{
	Free();
}

HRESULT CFireBall::Ready_Object(CTransform* pWeapon, CTransform* pOwner, _float _fSpeed)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_eObjectTag = OBJECTTAG::PLAYERBULLET;
	m_eState = STATE::ATTACK;
	//m_pTransform->Set_Parent(pOwner);
	m_pTransform->Scale(_vec3(0.5f, 0.5f, 0.5f));

	m_pCollider->InitOBB(
		m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		0.5f * m_pTransform->LocalScale());

	if (pWeapon != nullptr && pOwner != nullptr)
	{
		m_pTransform->m_vInfo[INFO_POS] = pWeapon->m_vInfo[INFO_POS];
		m_pTransform->Copy_RUL(pOwner->Get_Transform()->m_vInfo);

		m_vDir = pOwner->Get_Transform()->m_vInfo[INFO_LOOK];
		D3DXVec3Normalize(&m_vDir, &m_vDir);
	}
	//m_pTransform->Copy_RUL_AddPos(pOwner->Get_Transform()->m_vInfo);

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::ATTACK], STATE::ATTACK, 10.f, TRUE);
	m_pAnimator->Add_Animation(STATE::ATTACK, pAnimation);
	m_pAnimator->Set_Animation(STATE::ATTACK);


	BASICSTAT* pOwnerStat = {};
	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	pOwnerStat = rPlayer.Get_Stat()->Get_Stat();

	if (pOwnerStat != nullptr)
	{
		m_pBasicStat->Get_Stat()->iDamageMin = pOwnerStat->iDamageMin;
		m_pBasicStat->Get_Stat()->iDamageMax = pOwnerStat->iDamageMax;
	}

	// Åõ»çÃ¼ ÈçÀû ÀÌÆåÆ® Ãß°¡
	CGameObject* pGameObject = m_pEffect = CEffectProjectileTrace::Create(m_pGraphicDev);
	pGameObject->m_pTransform->Translate(m_pTransform->m_vInfo[INFO_POS]);
	dynamic_cast<CTempEffect*>(pGameObject)->Set_EffectColor(ECOLOR_PINK);
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);


	m_bIsAttack = false;
	m_vPrevPos = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

_int CFireBall::Update_Object(const _float& fTimeDelta)
{
	if (IsDead())
		return 0;

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()->
		Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());


	if (!m_bIsAttack)
	{
		m_vPrevPos = m_pTransform->m_vInfo[INFO_POS];
		m_bIsAttack = true;
	}

	_float fDistance = D3DXVec3Length(&(m_pTransform->m_vInfo[INFO_POS] - m_vPrevPos));

	if (fDistance > 60.f && Get_State() != STATE::DEAD)
	{
		Set_State(STATE::DEAD);
		EventManager()->DeleteObject(this);
	}


	m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS] + m_vDir * 25.f * fTimeDelta;

	m_pAnimator->Update_Animator(fTimeDelta);
	
	return iExit;
}

void CFireBall::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CFireBall::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pStateMachine->Render_StateMachine();
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	m_pAnimator->Render_Animator();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CFireBall::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	//__super::OnCollisionEnter(_pOther);

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

	if ((_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER &&
		dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_StateMachine()->Get_State() != STATE::DEAD) ||
		(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BOSS) ||
		(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BLOCK) ||
		(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::FRAGILE))
	{
		CGameObject* pGameObject = nullptr;
		if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER || _pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BOSS)
		{
			pPlayer.IsAttack(dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_BasicStat());

			//////////////////////////////////////////////////////////////////////////////// ÀÌÆåÆ® 
			_matrix      matMonsterWorld = _pOther->Get_Host()->m_pTransform->WorldMatrix();
			_vec3        vecMonsterPos = _vec3(matMonsterWorld._41, matMonsterWorld._42 + .5f, matMonsterWorld._43);
			pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_NONE);
			dynamic_cast<CEffectSquare*>(pGameObject)->Set_MonsterEffectColor(dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_MonsterTag());
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}

		pGameObject = CEffectExplosion::Create(m_pGraphicDev);
		pGameObject->m_pTransform->Translate(m_pTransform->m_vInfo[INFO_POS]);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		//////////////////////////////////////////////////////////////////////////////// ÀÌÆåÆ® 
	}

	Set_State(STATE::DEAD);
	EventManager()->DeleteObject(this);
}

void CFireBall::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CFireBall::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CFireBall::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture[(_uint)STATE::ATTACK] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_FireBullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

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

CFireBall* CFireBall::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pWeapon, 
	CTransform* pOwner, _float _fSpeed)
{
	CFireBall* pInstance = new CFireBall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pWeapon, pOwner, _fSpeed)))
	{
		Safe_Release<CFireBall*>(pInstance);

		MSG_BOX("Create FireBall Failed");
		return nullptr;
	}

	return pInstance;
}

void CFireBall::Free()
{
	Safe_Release(m_pEffect);

	__super::Free();
}
