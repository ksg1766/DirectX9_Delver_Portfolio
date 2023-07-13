#include "SpawnPattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Player.h"
#include "Skeleton.h"

CSpawnPattern::CSpawnPattern()
{

}

CSpawnPattern::CSpawnPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CSpawnPattern::~CSpawnPattern()
{
}

HRESULT CSpawnPattern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_bSkill = false;
	m_fSkillCool = 0.f;
	m_vSpawnPos[0] = _vec3(8.f, 0.f, 0.f);
	m_vSpawnPos[1] = _vec3(-8.f, 0.f, 8.f);
	m_vSpawnPos[2] = _vec3(0.f, 0.f, -8.f);
	m_vSpawnPos[3] = _vec3(0.f, 0.f, 8.f);
	return S_OK;
}

STATE CSpawnPattern::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fSkillCool += fTimeDelta;
	if ((1.f < m_fSkillCool)&&(!m_bSkill))
	{
		Spawn_Skeleton();
		m_bSkill = true;
	}
	if (3.f < m_fSkillCool)
	{
		m_bSkill = false;
		m_fSkillCool = 0.f;
		return STATE::BOSS_IDLE;
	}
}

void CSpawnPattern::LateUpdate_State()
{

}

void CSpawnPattern::Render_State()
{
}

void CSpawnPattern::Spawn_Skeleton()
{
	Engine::CGameObject* pGameObject = nullptr;
	for (int i = 0; i < 4; ++i)
	{
		pGameObject = CSkeleton::Create(m_pGraphicDev);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		dynamic_cast<CSkeleton*>(pGameObject)->Set_Terrain(dynamic_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Terrain());
		dynamic_cast<CSkeleton*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] =
			(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS]) + m_vSpawnPos[i];
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	}
}

CSpawnPattern* CSpawnPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CSpawnPattern* pState = new CSpawnPattern(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CSpawnPattern*>(pState);

		MSG_BOX("Boss SpawnPattern State Failed");
	}
	return pState;
}

void CSpawnPattern::Free()
{
	__super::Free();
}
