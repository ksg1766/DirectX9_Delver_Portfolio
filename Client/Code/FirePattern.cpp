#include "FirePattern.h"
#include "Export_Function.h"
#include "BossProjectile.h"
#include "SkeletonKing.h"

CFirePattern::CFirePattern()
{
}

CFirePattern::CFirePattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CFirePattern::~CFirePattern()
{
   
}

HRESULT CFirePattern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
    m_fDelay = 0.f;
    m_iSkillCount = 0;
	return S_OK;
}

STATE CFirePattern::Update_State(const _float& fTimeDelta)
{
   // m_fDelay += fTimeDelta;
    Engine::CGameObject* pGameObject = nullptr;

        m_fDelay += fTimeDelta;
        if (0.3f < m_fDelay)
        {
            pGameObject = CBossProjectile::Create(m_pGraphicDev);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
            dynamic_cast<CBossProjectile*>(pGameObject)->Set_Terrain(dynamic_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Terrain());
            dynamic_cast<CBossProjectile*>(pGameObject)->Set_Target(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
            ++m_iSkillCount;
            m_fDelay = 0.f;
        }
        if (2 < m_iSkillCount)
        {
            m_iSkillCount = 0;
            return STATE::BOSS_IDLE;
        }
}

void CFirePattern::LateUpdate_State()
{

}

void CFirePattern::Render_State()
{
}


CFirePattern* CFirePattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CFirePattern* pState = new CFirePattern(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CFirePattern*>(pState);

        MSG_BOX("Fire State Failed");
    }
    return pState;
}

void CFirePattern::Free()
{
    __super::Free();
}
