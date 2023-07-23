#include "FIreWave2.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "BossFireWave.h"

CFIreWave2::CFIreWave2()
{
}

CFIreWave2::CFIreWave2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CFIreWave2::~CFIreWave2()
{
}

HRESULT CFIreWave2::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;

	return S_OK;
}

STATE CFIreWave2::Update_State(const _float& fTimeDelta)
{
	return STATE();
}

void CFIreWave2::LateUpdate_State()
{
}

void CFIreWave2::Render_State()
{
}

void CFIreWave2::Create_Clone(_int _iNum)
{
}

CFIreWave2* CFIreWave2::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CFIreWave2* pState = new CFIreWave2(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CFIreWave2*>(pState);

		MSG_BOX("FireWave State Failed");
	}
	return pState;
}

void CFIreWave2::Free()
{
	__super::Free();
}
