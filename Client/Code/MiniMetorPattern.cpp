#include "MiniMetorPattern.h"
#include "MiniMeteor.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Boss_WarningEff.h"

CMiniMetorPattern::CMiniMetorPattern()
{
}

CMiniMetorPattern::CMiniMetorPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CMiniMetorPattern::~CMiniMetorPattern()
{
}

HRESULT CMiniMetorPattern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_iSkillCount = 0;
	m_fDelay = 0.f;
	m_bCool = false;
	m_CautionCool = false;
	m_iSpawnX = 0;
	return S_OK;
}

STATE CMiniMetorPattern::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fDelay += fTimeDelta;
	if ((0.2f < m_fDelay) && (!m_CautionCool))
	{
		mt19937 engine((_uint)time(NULL));           // MT19937 난수 엔진
		uniform_int_distribution<__int64> distribution(-18, 18); // 생성 범위
		auto generator = bind(distribution, engine);
		m_iSpawnX = generator();
		pGameObject = CBoss_WarningEff::Create(m_pGraphicDev);
		dynamic_cast<CBoss_WarningEff*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x + m_iSpawnX, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y + 5.f, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z + 5.f);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_fDelay = 0.f;
		m_CautionCool = true;
	}
	if ((0.2f < m_fDelay) && (m_CautionCool)&&(!m_bCool))
	{
		pGameObject = CMiniMeteor::Create(m_pGraphicDev);
		dynamic_cast<CMiniMeteor*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x + m_iSpawnX, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y + 5.f, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z + 5.f);
		dynamic_cast<CMiniMeteor*>(pGameObject)->Set_Dir(_vec3(0.f, 0.f, 0.3f));
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_bCool = true;
	}
	if ((0.2f < m_fDelay)&&(m_bCool)&&(m_CautionCool))
	{
		m_fDelay = 0.f;
		m_bCool = false;
		m_CautionCool = false;
		++m_iSkillCount;
	}
	if (20 <= m_iSkillCount)
	{
		m_fDelay = 0.f;
		m_bCool = false;
		m_iSkillCount = 0;
		return STATE::BOSS_IDLE;
	}
}

void CMiniMetorPattern::LateUpdate_State()
{
}

void CMiniMetorPattern::Render_State()
{
}

CMiniMetorPattern* CMiniMetorPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMiniMetorPattern* pState = new CMiniMetorPattern(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMiniMetorPattern*>(pState);

		MSG_BOX("MiniMeteor State Failed");
	}
	return pState;
}

void CMiniMetorPattern::Free()
{
	__super::Free();
}
