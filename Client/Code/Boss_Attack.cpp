#include "Boss_Attack.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "BossProjectile.h"
#include "BossExplosion.h"
#include "SkeletonKing_Clone.h"
CBoss_Attack::CBoss_Attack()
{
}

CBoss_Attack::CBoss_Attack(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CBoss_Attack::~CBoss_Attack()
{
}

HRESULT CBoss_Attack::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_vShotGunDir[0] = _vec3(-3.f, 0.f, -3.f);
    m_vShotGunDir[1] = _vec3(0.f, 0.f, 0.f);
    m_vShotGunDir[2] = _vec3(3.f, 0.f, 3.f);

    m_vExplosionin1[0] = _vec3(-2.f, 0.f, 2.f);
    m_vExplosionin1[1] = _vec3(2.f, 0.f, 2.f);
    m_vExplosionin1[2] = _vec3(-2.f, 0.f, -2.f);
    m_vExplosionin1[3] = _vec3(2.f, 0.f, -2.f);

   m_vExplosionin2[0] = _vec3(-2.f, 0.f, 0.f);
   m_vExplosionin2[1] = _vec3(0.f, 0.f, -2.f);
   m_vExplosionin2[2] = _vec3(2.f, 0.f, 0.f);
   m_vExplosionin2[3] = _vec3(0.f, 0.f, 2.f);

   m_bExplosion = false;
    return S_OK;
}

STATE CBoss_Attack::Update_State(const _float& fTimeDelta)
{
    return BossSkill(fTimeDelta);
}

void CBoss_Attack::LateUpdate_State()
{

}

void CBoss_Attack::Render_State()
{
}

STATE CBoss_Attack::BossSkill(const _float& fTimeDelta)
{
    Engine::CGameObject* pGameObject = nullptr;
    srand(unsigned(time(nullptr)));


    switch (rand() % 3)
    {
    case 0:
        return STATE::BOSS_FIRE;
        break;
    case 1:
       /* pGameObject = CBossExplosion::Create(m_pGraphicDev);
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPos(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
        dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPosY(-2.f);*/
        Explosion_Flower(fTimeDelta);
        return STATE::BOSS_IDLE;
        break;
    case 2:
        pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->Set_Terrain(dynamic_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Terrain());
        dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
        BossTeleport();
        
        return STATE::BOSS_IDLE;
        break;
    }
}

void CBoss_Attack::BossTeleport()
{
    srand(unsigned(time(nullptr)));
    _float fA;
    if (0 == rand() % 2)
        fA = -1.f;
    else 
        fA = 1.f;

    _vec3   vDestination;
    vDestination.x = 5 * cosf((_float)rand() /D3DXToRadian(15)) * fA;
    vDestination.y = 0.f;
    vDestination.z = 5 * -sinf((_float)rand() / D3DXToRadian(15))* fA;
    m_pOwner->Get_Transform()->m_vInfo[INFO_POS] += vDestination;
   // m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y = 1.0f;
}

void CBoss_Attack::FireShogun(const _float& fTimeDelta)
{
    Engine::CGameObject* pGameObject = nullptr;

    for (int i = 0; i < 3; ++i)
    {
        pGameObject = CBossProjectile::Create(m_pGraphicDev);//여기서 오류
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        dynamic_cast<CBossProjectile*>(pGameObject)->Set_Terrain(dynamic_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Terrain());
        dynamic_cast<CBossProjectile*>(pGameObject)->Set_Target(m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + m_vShotGunDir[i]);
    }
}

void CBoss_Attack::Explosion_Flower(const _float& fTimeDelta)
{
    Engine::CGameObject* pGameObject = nullptr;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 1; j < 4; ++j)
            {
                pGameObject = CBossExplosion::Create(m_pGraphicDev);
                Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
                dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPos(m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + (m_vExplosionin1[i] * j));
                dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPosY(-2.f);
            }
        }
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 1; j < 4; ++j)
            {
                pGameObject = CBossExplosion::Create(m_pGraphicDev);
                Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
                dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPos(m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + (m_vExplosionin2[i] * j));
                dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPosY(-2.f);
            }
        }
}

CBoss_Attack* CBoss_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_Attack* pState = new CBoss_Attack(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_Attack*>(pState);

        MSG_BOX("Boss Attack State Failed");
    }
    return pState;
}

void CBoss_Attack::Free()
{
    __super::Free();
}
