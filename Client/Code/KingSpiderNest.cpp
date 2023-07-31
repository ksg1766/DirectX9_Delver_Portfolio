#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpiderNest.h"
#include "Export_Function.h"
#include "Player.h"
#include "DungeonSpider.h"
#include "EffectSquare.h"
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
	m_bSpawn = false;
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

	if ((4.f > m_fScale) &&(2.f < m_fDelay))
	{
		m_fDelay += fTimeDelta;
		if (4.f <= m_fScale)
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
		m_IsDead = true;
	}
	return iExit;
}

void CKingSpiderNest::LateUpdate_Object(void)
{
}

void CKingSpiderNest::Render_Object(void)
{
}

void CKingSpiderNest::Init_Stat()
{
}

void CKingSpiderNest::OnCollisionEnter(CCollider* _pOther)
{
}

void CKingSpiderNest::OnCollisionStay(CCollider* _pOther)
{
}

void CKingSpiderNest::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CKingSpiderNest::Add_Component(void)
{
	return E_NOTIMPL;
}

CKingSpiderNest* CKingSpiderNest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CKingSpiderNest::Free()
{
}
