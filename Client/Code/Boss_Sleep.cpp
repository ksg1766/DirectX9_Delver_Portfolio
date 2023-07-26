#include "Boss_Sleep.h"
#include "Export_Function.h"

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
	return S_OK;
}

STATE CBoss_Sleep::Update_State(const _float& fTimeDelta)
{
	//해당 상태일 때 업데이트 할 것들
	_vec3 vPlayerPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
	_vec3 vBossPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	_vec3 vDir = vPlayerPos - vBossPos;
	_float fDistance = D3DXVec3LengthSq(&vDir);
	if (fDistance < pow(20, 2))
	{
		return STATE::BOSS_WAKEUP;
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
