#include "..\Header\Arrow.h"
#include "Export_Function.h"
#include "Player.h"

#include "Itemgroup.h"

#include "EffectSquare.h"
#include "EffectDamageStar.h"
#include "EffectDamage.h"
#include "EffectPastTrace.h"

static _int g_iCount = 0;

CArrow::CArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
{
}

CArrow::CArrow(const CArrow& rhs)
	: CItem(rhs)
{

}

CArrow::~CArrow()
{
	Free();
}

HRESULT CArrow::Ready_Object(CTransform* Weapon, CTransform* pOwner, _float _fSpeed)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_eObjectTag = OBJECTTAG::PLAYERBULLET;
	m_eState = STATE::ATTACK;
	m_fSpeed = _fSpeed;

	//m_pTransform->Set_Parent(pOwner);

	if (Weapon != nullptr && pOwner != nullptr)
	{
		m_pTransform->Copy_RUL(pOwner->Get_Transform()->m_vInfo);
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
		m_pTransform->Rotate(ROT_Y, -30.f);

		m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], 0.5f * m_pTransform->LocalScale());
		m_pTransform->m_vInfo[INFO_POS] = Weapon->m_vInfo[INFO_POS];

		m_vDir = pOwner->Get_Transform()->m_vInfo[INFO_LOOK];
		D3DXVec3Normalize(&m_vDir, &m_vDir);
	}

	BASICSTAT* pOwnerStat = {};

	if (dynamic_cast<CItem*>(Weapon->Get_Host())->Get_ItemTag().eItemID == ITEMID::WEAPON_BOW)
		pOwnerStat = dynamic_cast<CBow*>(Weapon->Get_Host())->Get_ItemStat()->Get_Stat();
	else
		pOwnerStat = dynamic_cast<CEpicBow*>(Weapon->Get_Host())->Get_ItemStat()->Get_Stat();


	if (pOwnerStat != nullptr)
	{
		m_pBasicStat->Get_Stat()->iDamageMin = pOwnerStat->iDamageMin;
		m_pBasicStat->Get_Stat()->iDamageMax = pOwnerStat->iDamageMax;
	}

	m_bIsAttack = false;
	m_vPrevPos = _vec3(0.f, 0.f, 0.f);

	// 투사체 흔적 이펙트 추가
	CGameObject* pGameObject = m_pEffect = CEffectPastTrace::Create(m_pGraphicDev);
	pGameObject->m_pTransform->Translate(m_pTransform->m_vInfo[INFO_POS]);
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

	return S_OK;
}

_int CArrow::Update_Object(const _float& fTimeDelta)
{

	if (IsDead())
		return 0;

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	
	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);		

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

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


	m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS] + m_vDir * m_fSpeed * fTimeDelta;

	return iExit;
}

void CArrow::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	//__super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CArrow::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

HRESULT CArrow::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Arrow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CArrow::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::MONSTER &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER &&
		_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM)
		__super::OnCollisionEnter(_pOther);
	// 몬스터거나 플레이어면 밀어내지않는다.

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()
		->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	// 플레이어의 정보를 레퍼런스로 얻어옴.

	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER &&
		dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_StateMachine()->Get_State() != STATE::DEAD)
	{
		pPlayer.IsAttack(dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_BasicStat());
	

		if (++g_iCount == 2)
		{
			dynamic_cast<CMonster*>(_pOther->Get_Host())->Set_KnockBack(true);
			g_iCount = 0;

			//////////////////////////////////////////////////////////////////////////////// 이펙트 
			CGameObject* pGameObject = CEffectDamageStar::Create(m_pGraphicDev);
			dynamic_cast<CTempEffect*>(pGameObject)->Set_TargetObject(_pOther->Get_Host());
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			//////////////////////////////////////////////////////////////////////////////// 이펙트 

			Engine::EventManager()->DeleteObject(this);
		}

		//////////////////////////////////////////////////////////////////////////////// 이펙트 
		_matrix      matMonsterWorld = _pOther->GetHost()->m_pTransform->WorldMatrix();
		_vec3        vecMonsterPos = _vec3(matMonsterWorld._41, matMonsterWorld._42 + .5f, matMonsterWorld._43);
		CGameObject* pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_NONE);
		dynamic_cast<CEffectSquare*>(pGameObject)->Set_MonsterEffectColor(dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_MonsterTag());
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		//////////////////////////////////////////////////////////////////////////////// 이펙트 

		Set_State(STATE::DEAD);
		Engine::EventManager()->DeleteObject(this);
	}

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BLOCK || _pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::FRAGILE)
		EventManager()->DeleteObject(this);
}

void CArrow::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CArrow::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

CArrow* CArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* Weapon,
	CTransform* pOwner, _float _fSpeed)
{
	CArrow* pInstance = new CArrow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Weapon, pOwner, _fSpeed)))
	{
		Safe_Release<CArrow*>(pInstance);

		MSG_BOX("Create Arrow Failed");
		return nullptr;
	}

	return pInstance;
}

void CArrow::Free()
{
	Safe_Release(m_pEffect);

	__super::Free();
}
