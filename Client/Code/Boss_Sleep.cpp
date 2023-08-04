#include "Boss_Sleep.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "SoundManager.h"
#include "Skeleton.h"
CBoss_Sleep::CBoss_Sleep()
{
}

CBoss_Sleep::CBoss_Sleep(LPDIRECT3DDEVICE9 pGrapicDev)
	:CState(pGrapicDev)
{
}

CBoss_Sleep::~CBoss_Sleep()
{
}

HRESULT CBoss_Sleep::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_bMusic = false;
	m_bSummon = false;

	m_vSummonPos[0] = _vec3(-6.f, 0.f, 6.f);//보스 우측상단
	m_vSummonPos[1] = _vec3(-6.f, 0.f, -6.f);//보스 좌측상단
	m_vSummonPos[2] = _vec3(6.f, 0.f, 6.f);//보스 우측하단
	m_vSummonPos[3] = _vec3(6.f, 0.f, -6.f);//보스 좌측하단
	m_vSummonPos[4] = _vec3(-6.f, 0.f, 0.f);
	m_vSummonPos[5] = _vec3(0.f, 0.f, -6.f);
	m_vSummonPos[6] = _vec3(6.f, 0.f, 0.f);
	m_vSummonPos[7] = _vec3(0.f, 0.f, 6.f);
	return S_OK;
}

STATE CBoss_Sleep::Update_State(const _float& fTimeDelta)
{
	//해당 상태일 때 업데이트 할 것들
	_vec3 vPlayerPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
	_vec3 vBossPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	_vec3 vDir = vPlayerPos - vBossPos;
	_float fDistance = D3DXVec3LengthSq(&vDir);
	if (BOSSPHASE::PHASE1 == static_cast<CSkeletonKing*>(m_pOwner->Get_Host())->Get_Phase())
	{
		if ((!m_bMusic)&&(fDistance < pow(30, 2)))
		{
			CSoundManager::GetInstance()->StopAll();
			//CSoundManager::GetInstance()->PlayBGM(L"Test_BossTheme.mp3", 0.3f);
			CSoundManager::GetInstance()->PlayBGM(L"AstalgiaofPhantam", 1.f);
			m_bMusic = true;
		}
		if (fDistance < pow(20, 2))
		{
			return STATE::BOSS_WAKEUP;
		}
	}
	else if (BOSSPHASE::PHASE2 == static_cast<CSkeletonKing*>(m_pOwner->Get_Host())->Get_Phase())
	{
		if (fDistance < pow(40, 2))
		{
			return STATE::BOSS_WAKEUP;
		}
	}
	else if (BOSSPHASE::PHASE3 == static_cast<CSkeletonKing*>(m_pOwner->Get_Host())->Get_Phase())
	{
		static_cast<CSkeletonKing*>(m_pOwner->Get_Host())->Set_3Phase(true);
		if (fDistance < pow(40, 2))
		{
			return STATE::BOSS_WAKEUP;
		}
	}
	else if (BOSSPHASE::LASTPHASE == static_cast<CSkeletonKing*>(m_pOwner->Get_Host())->Get_Phase())
	{
		//여기에 해골들소환
		if (!m_bSummon)
		{
			CGameObject* pGameObject = nullptr;
			for (int i = 0; i < 8; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					pGameObject = CSkeleton::Create(m_pGraphicDev);
					static_cast<CSkeleton*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + (m_vSummonPos[i]*(j+1));
					static_cast<CSkeleton*>(pGameObject)->Get_BasicStat()->Get_Stat()->iExpMax = 0.f;
					static_cast<CSkeleton*>(pGameObject)->Get_BasicStat()->Get_Stat()->iExp = 0.f;
					static_cast<CSkeleton*>(pGameObject)->Get_BasicStat()->Get_Stat()->fHP = 3.f;
					static_cast<CSkeleton*>(pGameObject)->Set_CenterPos(_vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x + m_vSummonPos[i].x / 2, 35.f, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z + m_vSummonPos[i].z / 2));
					static_cast<CSkeleton*>(pGameObject)->Set_MoveRange(5.f);
					Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
				}

			}
			m_bSummon = true;
		}

		if (fDistance < pow(25, 2))
		{
			return STATE::BOSS_CRAWL;
		}
	}
	return STATE::BOSS_SLEEP;
}

void CBoss_Sleep::LateUpdate_State()
{
}

void CBoss_Sleep::Render_State()
{
	//테스트 상태로그 확인용 출력(cout)
}

CBoss_Sleep* CBoss_Sleep::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CBoss_Sleep* pState = new CBoss_Sleep(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CBoss_Sleep*>(pState);

		MSG_BOX("BossSleep State Failed");
	}
	return pState;
}

void CBoss_Sleep::Free()
{
	__super::Free();
}
