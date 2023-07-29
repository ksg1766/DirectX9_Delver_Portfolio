#include "Boss_Sleep.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "SoundManager.h"
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
	return S_OK;
}

STATE CBoss_Sleep::Update_State(const _float& fTimeDelta)
{
	//해당 상태일 때 업데이트 할 것들
	_vec3 vPlayerPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
	_vec3 vBossPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	_vec3 vDir = vPlayerPos - vBossPos;
	_float fDistance = D3DXVec3LengthSq(&vDir);
	if (BOSSPHASE::PHASE1 == dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Phase())
	{
		if ((!m_bMusic)&&(fDistance < pow(100, 2)))
		{
			CSoundManager::GetInstance()->StopAll();
			CSoundManager::GetInstance()->PlayBGM(L"Test_BossTheme.mp3", 0.3f);
			m_bMusic = true;
		}
		if (fDistance < pow(20, 2))
		{
			return STATE::BOSS_WAKEUP;
		}
	}
	else if (BOSSPHASE::PHASE2 == dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Phase())
	{
		if (fDistance < pow(40, 2))
		{
			return STATE::BOSS_WAKEUP;
		}
	}
	else if (BOSSPHASE::PHASE3 == dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Phase())
	{
		if (fDistance < pow(40, 2))
		{
			return STATE::BOSS_WAKEUP;
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
