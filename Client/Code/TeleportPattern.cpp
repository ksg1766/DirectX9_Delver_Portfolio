#include "..\Header\TeleportPattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "SkeletonKing_Clone.h"
#include "Boss_BatSwarm.h"
#include "Player.h"
CTeleportPattern::CTeleportPattern()
{
}

CTeleportPattern::CTeleportPattern(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CTeleportPattern::~CTeleportPattern()
{
}

HRESULT CTeleportPattern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
    m_fDelay = 0.f;
    m_bSkillCount = false;
    m_fBatMovePos = _vec3(-10.f, 0.f , -10.f);

	return S_OK;
}

STATE CTeleportPattern::Update_State(const _float& fTimeDelta)
{
    m_fDelay += fTimeDelta;
    if (2.5f < m_fDelay)
    {
        m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = m_fDestination;
        m_bSkillCount = false;
        m_fDelay = 0.f;
        return STATE::BOSS_IDLE;
    }

    else if (2.f < m_fDelay)
    {
        _vec3   vDestination;
        vDestination.x = 5 * cosf((_float)rand() / D3DXToRadian(15));// *fA;
        vDestination.y = 0.f;
        vDestination.z = 5 * -sinf((_float)rand() / D3DXToRadian(15));// *fA;

        m_fDestination = (Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS]) + vDestination;
    }
    else
    {
        Make_BatSwarm(fTimeDelta);
    }
    m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y = -99.f;
}

void CTeleportPattern::LateUpdate_State()
{

}

void CTeleportPattern::Render_State()
{
    
}

void CTeleportPattern::Make_BossClone()
{
    if (m_bSkillCount)
        return;
        
        Engine::CGameObject* pGameObject = nullptr;
        pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->Set_Terrain(dynamic_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Terrain());
        dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
        m_bSkillCount = true;

}

void CTeleportPattern::Make_BatSwarm(const _float& fTimeDelta)
{

   Engine::CGameObject* pGameObject = nullptr;
   for (int i = 3; i < 7; ++i)
   {
     pGameObject =  CBoss_BatSwarm::Create(m_pGraphicDev);
     Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
     dynamic_cast<CBoss_BatSwarm*>(pGameObject)->Set_StartPos(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
     dynamic_cast<CBoss_BatSwarm*>(pGameObject)->Add_Angle(i*10);
   }
}

CTeleportPattern* CTeleportPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CTeleportPattern* pState = new CTeleportPattern(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CTeleportPattern*>(pState);

        MSG_BOX("Boss Bunshin State Failed");
    }
    return pState;
}

void CTeleportPattern::Free()
{
    __super::Free();
}
