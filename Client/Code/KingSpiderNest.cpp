#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpiderNest.h"
#include "Export_Function.h"
#include "Player.h"
#include "DungeonSpider.h"
#include "EffectSquare.h"
#include "Item.h"
CKingSpiderNest::CKingSpiderNest(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{
}

CKingSpiderNest::CKingSpiderNest(const CKingSpiderNest& rhs)
	: Engine::CMonster(rhs)
{
}

CKingSpiderNest::~CKingSpiderNest()
{
	Free();
}

HRESULT CKingSpiderNest::Ready_Object(void)
{
	Set_ObjectTag(OBJECTTAG::MONSTERBULLET);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fScale = 1.f;
	m_fDelay = 0.f;
	m_iHp = 5.f;
	m_fHitCool = 0.f;
	m_bSpawn = false;
	m_bMaxSize = false;
	m_bHit = false;
	m_vSpawnPos[0] = _vec3(-1.f, 0.f, -1.f);
	m_vSpawnPos[1] = _vec3(1.f, 0.f, -1.f);
	m_vSpawnPos[2] = _vec3(-1.f, 0.f, 1.f);
	m_vSpawnPos[3] = _vec3(1.f, 0.f, 1.f);
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	return S_OK;
}

_int CKingSpiderNest::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);
	if (m_bHit)
	{
		m_fHitCool += fTimeDelta;
		if (0.5f < m_fHitCool)
		{
			m_fHitCool = 0.f;
			m_bHit = false;
		}
	}
	if ((4.f >= m_fScale) &&(1.5f < m_fDelay))
	{
		m_fDelay += fTimeDelta;
		if (4.f >= m_fScale)
			m_fScale += fTimeDelta;
		m_pTransform->Scale(_vec3(m_fScale, m_fScale, m_fScale));
		m_fDelay = 0.f;
	}
	else if ((4.f < m_fScale) && (!m_bSpawn))
	{
		Engine::CGameObject* pGameObject = nullptr;

		_matrix      matMonsterWorld = m_pTransform->WorldMatrix();
		_vec3        vecMonsterPos = _vec3(matMonsterWorld._41, matMonsterWorld._42 + .5f, matMonsterWorld._43);
		pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_APRICOT);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		
		for (int i = 0; i < 4; ++i)
		{
			pGameObject = CDungeonSpider::Create(m_pGraphicDev);
			dynamic_cast<CDungeonSpider*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS] + m_vSpawnPos[i];
			dynamic_cast<CDungeonSpider*>(pGameObject)->Get_RigidBody()->Add_Force(_vec3(0.f,2.f,0.f));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_bMaxSize = true;
		m_IsDead = true;
	}
	return iExit;
}

void CKingSpiderNest::LateUpdate_Object(void)
{
}

void CKingSpiderNest::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(18);
	m_pBuffer->Render_Buffer();

	//////////////////////////////////////////////////////////////////////
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(90.f));
	_matrix matWorld = m_pTransform->WorldMatrix();
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(-90.f));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pTexture->Render_Texture(18);
	m_pBuffer->Render_Buffer();
}

void CKingSpiderNest::Init_Stat()
{
}

void CKingSpiderNest::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::OnCollisionEnter(_pOther);
}

void CKingSpiderNest::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::OnCollisionStay(_pOther);
	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();
	
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::ITEM && !IsDead())
	{
		if ((dynamic_cast<CItem*>(_pOther->Get_Host())->
			Get_ItemTag().eItemType == ITEMTYPE::ITEMTYPE_WEAPONITEM &&
			!m_bHit && rPlayer.Get_Attack()))
		{
			m_iHp -= 1.f;
			m_bHit = true;

			Engine::CGameObject* pGameObject = nullptr;

			_matrix      matMonsterWorld = m_pTransform->WorldMatrix();
			_vec3        vecMonsterPos = _vec3(matMonsterWorld._41, matMonsterWorld._42 + .5f, matMonsterWorld._43);
			pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_APRICOT);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			if (m_iHp <= 0)
			{
				EventManager()->DeleteObject(this);
			}

		}
	}

	else if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYERBULLET && !IsDead())
	{
		m_iHp -= 1.f;
		m_bHit = true;
		Engine::CGameObject* pGameObject = nullptr;

		_matrix      matMonsterWorld = m_pTransform->WorldMatrix();
		_vec3        vecMonsterPos = _vec3(matMonsterWorld._41, matMonsterWorld._42 + .5f, matMonsterWorld._43);
		pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_APRICOT);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		if (m_iHp <= 0)
		{
			EventManager()->DeleteObject(this);
		}
	}
}

void CKingSpiderNest::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::OnCollisionExit(_pOther);
}

HRESULT CKingSpiderNest::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_ImmortalSprite"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CKingSpiderNest* CKingSpiderNest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CKingSpiderNest* pInstance = new CKingSpiderNest(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("KingSpiderNest Create Failed");
		return nullptr;
	}

	return pInstance;

}

void CKingSpiderNest::Free()
{
	__super::Free();
}
