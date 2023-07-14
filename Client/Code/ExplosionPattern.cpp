#include "ExplosionPattern.h"
#include "Export_Function.h"
#include "BossExplosion.h"
#include "SkeletonKing.h"

CExplosionPattern::CExplosionPattern()
{
}

CExplosionPattern::CExplosionPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CExplosionPattern::~CExplosionPattern()
{
}

HRESULT CExplosionPattern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
    m_fDelay = 0.f;
    m_iSkillCount = 0;
    m_bPattern = false;

    m_vExplosionin1[0] = _vec3(-2.f, 0.f, 2.f);
    m_vExplosionin1[1] = _vec3(2.f, 0.f, 2.f);
    m_vExplosionin1[2] = _vec3(-2.f, 0.f, -2.f);
    m_vExplosionin1[3] = _vec3(2.f, 0.f, -2.f);

    m_vExplosionin2[0] = _vec3(-2.f, 0.f, 0.f);
    m_vExplosionin2[1] = _vec3(0.f, 0.f, -2.f);
    m_vExplosionin2[2] = _vec3(2.f, 0.f, 0.f);
    m_vExplosionin2[3] = _vec3(0.f, 0.f, 2.f);
	return S_OK;
}

STATE CExplosionPattern::Update_State(const _float& fTimeDelta)
{
    Engine::CGameObject* pGameObject = nullptr;
    m_fDelay += fTimeDelta;

    if (!m_bPattern)
    {
        if (0.5f < m_fDelay)
        {
            for (int i = 0; i < 4; ++i)
            {
                pGameObject = CBossExplosion::Create(m_pGraphicDev);
                dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPos(m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + (m_vExplosionin1[i] * m_iSkillCount));
                Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
                //dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPosY(-2.f);
            }
            ++m_iSkillCount;
            m_fDelay = 0.f;
        }
        if (3 < m_iSkillCount)
        {
            m_fDelay = 0.f;
            m_iSkillCount = 0.f;
            m_bPattern = true;
        }
    }
    else if (m_bPattern)
    {
        if (0.5f < m_fDelay)
        {
            for (int i = 0; i < 4; ++i)
            {
                pGameObject = CBossExplosion::Create(m_pGraphicDev);
                dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPos(m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + (m_vExplosionin2[i] * m_iSkillCount));
                Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
                //dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPosY(-2.f);
            }
            ++m_iSkillCount;
            m_fDelay = 0.f;
        }
        if (3 < m_iSkillCount)
        {
            m_fDelay = 0.f;
            m_iSkillCount = 0.f;
            m_bPattern = false;
            //return STATE::BOSS_IDLE;
            return STATE::BOSS_SPWANMONSTER;
        }
    }
}

void CExplosionPattern::LateUpdate_State()
{

}

void CExplosionPattern::Render_State()
{

}


CExplosionPattern* CExplosionPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CExplosionPattern* pState = new CExplosionPattern(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CExplosionPattern*>(pState);

        MSG_BOX("Explosion Pattern State Failed");
    }
    return pState;
}

void CExplosionPattern::Free()
{
    __super::Free();
}
