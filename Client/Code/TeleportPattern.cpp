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
    m_fSpawnHeight = 0.f;
    m_fSpawnWeight = 0.4f;
    m_fBatMovePos[0]=_vec3(0.5f, 0.f, 0.5f);
    m_fBatMovePos[1]=_vec3(0.5f, 0.f, -0.5f);
    m_fBatMovePos[2]=_vec3(-0.5f, 0.f, 0.5f);
    m_fBatMovePos[3]=_vec3(-0.5f, 0.f, -0.5f);

    m_fBatMovePos[4] = _vec3(0.5f, 0.f, 0.f);
    m_fBatMovePos[5] = _vec3(-0.5f, 0.f, 0.f);
    m_fBatMovePos[6] = _vec3(0.f, 0.f, 0.5f);
    m_fBatMovePos[7] = _vec3(0.f, 0.f, -0.5f);

	return S_OK;
}

STATE CTeleportPattern::Update_State(const _float& fTimeDelta)
{
    m_fDelay += fTimeDelta;
    switch (dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Phase())
    {
    case BOSSPHASE::PHASE2:
        if ((1.5f >= fabs(-72.5f - m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x)) && (1.5f >= fabs(94.5f - m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z)))
        {
            m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = _vec3(-72.5f, 38.f, 94.5f);
            return STATE::BOSS_SLEEP;
        }
        m_vDir = _vec3(-72.5f, 38.f, 94.5f) - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        m_pOwner->Get_Transform()->Translate(m_vDir);
        m_fDelay = 0.f;
        //return STATE::BOSS_SLEEP //바로 하니까 애가 이동을 안하지 병신아
        break;

    case BOSSPHASE::PHASE3:
        if ((-72.f >= fabs(m_vDir.x - m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x)) && (1.5f >= fabs(-105.f - m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z)))
        {
            m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = _vec3(-72.f, 34.f, -105.f);
            return STATE::BOSS_SLEEP;
        }
        m_vDir = _vec3(-72.f, 33.f, -105.f) - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        m_pOwner->Get_Transform()->Translate(m_vDir);
        m_fDelay = 0.f;
        break;
    }
}

void CTeleportPattern::LateUpdate_State()
{

}

void CTeleportPattern::Render_State()
{
}

void CTeleportPattern::Make_BatSwarm(const _float& fTimeDelta)
{

 

 
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
