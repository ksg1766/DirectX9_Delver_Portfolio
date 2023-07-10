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
        return STATE::BOSS_EXPLOSION;
        break;
    case 2:
        return STATE::BOSS_TELEPORT;
        break;
    }
    //  return STATE::BOSS_CRAWL;
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
