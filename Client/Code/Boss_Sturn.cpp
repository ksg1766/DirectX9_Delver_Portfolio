#include "Boss_Sturn.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Boss_SturnEffect.h"
#include "Boss_MeteorReady.h"
CBoss_Sturn::CBoss_Sturn()
{
}

CBoss_Sturn::CBoss_Sturn(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CBoss_Sturn::~CBoss_Sturn()
{
}

HRESULT CBoss_Sturn::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_fCount = 0.f;
    m_bStar = false;
    return S_OK;
}

STATE CBoss_Sturn::Update_State(const _float& fTimeDelta)
{
    m_fCount += fTimeDelta;
    if (!m_bStar)
    {
        Engine::CGameObject* pGameObject = nullptr;
        pGameObject = CBoss_SturnEffect::Create(m_pGraphicDev);
        dynamic_cast<CBoss_SturnEffect*>(pGameObject)->m_pTransform->Translate(_vec3(3.f,0.f,0.f));
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        m_bStar = true;
    }
    if (10.f < m_fCount)
    {
        dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->ReSet_Sturn();
        dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Set_Sturn(false);
        m_bStar = false;
        m_fCount = 0.f;
        if ((45 > dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_BasicStat()->Get_Stat()->fHP)
            && (BOSSPHASE::PHASE2 == dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Phase()))
        {
            dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Set_Phase(BOSSPHASE::PHASE3);
            return STATE::BOSS_TELEPORT;
        }
        return STATE::BOSS_IDLE;
    }
    dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Set_Sturn(true);
    return STATE::BOSS_STURN;
}

void CBoss_Sturn::LateUpdate_State()
{
}

void CBoss_Sturn::Render_State()
{
    //cout << "보스 스턴중 " << endl;
}

CBoss_Sturn* CBoss_Sturn::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_Sturn* pState = new CBoss_Sturn(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_Sturn*>(pState);

        MSG_BOX("Boss Sturn State Failed");
    }
    return pState;
}

void CBoss_Sturn::Free()
{
    __super::Free();
}
