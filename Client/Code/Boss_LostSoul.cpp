#include "stdafx.h"
#include "..\Header\Boss_LostSoul.h"
#include "Export_Function.h"
#include "Player.h"
#include "Shield.h"
#include "SkeletonKing.h"
#include "BossExplosion.h"
#include "EffectExplosion.h"
#include "SoundManager.h"
#include "LostSoul_State.h"
#include "EffectSquare.h"

CBossLostSoul::CBossLostSoul(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev), m_fFrame(0.f)
{
}

CBossLostSoul::CBossLostSoul(const CBossLostSoul& rhs)
	:	Engine::CMonster(rhs), m_fFrame(rhs.m_fFrame)
{
}

CBossLostSoul::~CBossLostSoul()
{
	Free();
}

HRESULT CBossLostSoul::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::LOSTSOUL;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS]
		, &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale()*0.5f);
	m_pBasicStat->Get_Stat()->fAttack = 1.f;
	m_fTime = 0.f;
	m_fSpeed = 1.5f;
	m_bHit = false;
	m_bParry = false;
	m_eSoulState = SOULSTATE::SOUL_NORMAL;
	CState* m_pState = CLostSoul_State::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, m_pState);
	m_pStateMachine->Set_State(STATE::IDLE);
	CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SPIDER);
	CSoundManager::GetInstance()->PlaySound(L"LostSoulSpawn1.wav", CHANNELID::SOUND_SPIDER, 0.8f);
	return S_OK;
}

_int CBossLostSoul::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);
	m_fTime += fTimeDelta;
		if (!m_bHit)//(5.f < m_fTime)
		{
			if ((m_eSoulState == SOULSTATE::SOUL_NORMAL) &&(5.f < m_fTime))
			{
				m_fTime = 0.f;
				Engine::CGameObject* pGameObject = nullptr;
				pGameObject = CBossExplosion::Create(m_pGraphicDev);
				dynamic_cast<CBossExplosion*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS];
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
				dynamic_cast<CBossExplosion*>(pGameObject)->Set_Scale(1.2f);
				Engine::EventManager()->DeleteObject(this);
			}
			else
			{
				_vec3	vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
				m_vDir = vTargetPos - m_pTransform->m_vInfo[INFO_POS];
				m_pTransform->Translate(m_vDir* m_fSpeed * fTimeDelta);
			}
			m_fFrame += 4.f * fTimeDelta * 2;
			if (4.f < m_fFrame)
				m_fFrame = 0.f;
		}
		else if (m_eSoulState == SOULSTATE::SOUL_PARRY)
		{
			_vec3	vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS];
			m_vDir = vTargetPos - m_pTransform->m_vInfo[INFO_POS];
			m_pTransform->Translate(m_vDir * m_fSpeed * fTimeDelta);
			m_fFrame += 6.f * fTimeDelta * 2;
			if (6.f < m_fFrame)
				m_fFrame = 4.f;
		}

	return iExit;
}

void CBossLostSoul::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CBossLostSoul::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pTexture->Render_Texture((_uint)m_fFrame);
	m_pBuffer->Render_Buffer();
}

void CBossLostSoul::Init_Stat()
{
}

void CBossLostSoul::Set_Target(_vec3 _vPos)
{
	m_pTransform->m_vInfo[INFO_POS] = _vPos;
	m_pTransform->Translate(_vec3(1.f, 3.f, 0.f));
	m_vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
	m_vDir = m_vTargetPos - m_pTransform->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}

void CBossLostSoul::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CBossLostSoul::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		if (m_eSoulState == SOULSTATE::SOUL_NORMAL)
		{
			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SPIDER);
			CSoundManager::GetInstance()->PlaySound(L"LostSoul3.wav", CHANNELID::SOUND_SPIDER, 1.f);
			CPlayerStat& PlayerState = *(dynamic_cast<CPlayer*>(_pOther->Get_Host())->Get_Stat());
			PlayerState.Take_Damage(this->Get_BasicStat()->Get_Stat()->fAttack);
			this->Set_AttackTick(true);
			Engine::CGameObject* pGameObject = nullptr;
			pGameObject = CBossExplosion::Create(m_pGraphicDev);
			dynamic_cast<CBossExplosion*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS];
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			dynamic_cast<CBossExplosion*>(pGameObject)->Set_Scale(1.2f);
			Engine::EventManager()->DeleteObject(this);
		}
	}
	else if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::ITEM)
	{
		if ((m_eSoulState == SOULSTATE::SOUL_NORMAL))
		{
			if ((ITEMID::GENERAL_SHIELD == dynamic_cast<CItem*>(_pOther->Get_Host())->Get_ItemTag().eItemID))
			{
				if (!dynamic_cast<CPlayer*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front())->Get_Parrying())
					return;

				_matrix      matMonsterWorld = _pOther->Get_Host()->m_pTransform->WorldMatrix();
				_vec3        vecMonsterPos = _vec3(matMonsterWorld._41, matMonsterWorld._42 + .5f, matMonsterWorld._43);
				CGameObject* pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_WHITE);
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

				CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SPIDER);
				CSoundManager::GetInstance()->PlaySound(L"Parry1.wav", CHANNELID::SOUND_SPIDER, 1.f);
				m_eSoulState = SOULSTATE::SOUL_PARRY;
				m_bHit = true;
				m_bParry = true;
				m_fFrame = 4.f;
			}
		}
		else
			return;
	}

	if (m_eSoulState == SOULSTATE::SOUL_PARRY)
	{
		if ((_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BOSS))
		{
			_matrix      matMonsterWorld = _pOther->Get_Host()->m_pTransform->WorldMatrix();
			_vec3        vecMonsterPos = _vec3(matMonsterWorld._41, matMonsterWorld._42 + .5f, matMonsterWorld._43);
			CGameObject* pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_APRICOT);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_SPIDER);
			CSoundManager::GetInstance()->PlaySound(L"LostSoul2.wav", CHANNELID::SOUND_SPIDER, 1.f);
			
			pGameObject = CEffectExplosion::Create(m_pGraphicDev);
			dynamic_cast<CEffectExplosion*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS];
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			pGameObject = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front();//보스데미지주기
			dynamic_cast<CSkeletonKing*>(pGameObject)->Add_HitCount();
			Engine::EventManager()->DeleteObject(this);
		}
	}
	else
		return;
}

void CBossLostSoul::OnCollisionExit(CCollider* _pOther)
{

}

HRESULT CBossLostSoul::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_LostSoul"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
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

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBossLostSoul* CBossLostSoul::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossLostSoul* pInstance = new CBossLostSoul(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BossLostSoul Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBossLostSoul::Free()
{
	__super::Free();
}
