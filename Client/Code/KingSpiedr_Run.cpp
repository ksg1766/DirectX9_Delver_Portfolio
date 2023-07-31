#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpiedr_Run.h"
#include "Export_Function.h"
#include "Player.h"

CKingSpiedr_Run::CKingSpiedr_Run()
{
}

CKingSpiedr_Run::CKingSpiedr_Run(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpiedr_Run::~CKingSpiedr_Run()
{
}

HRESULT CKingSpiedr_Run::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;


	return S_OK;
}

STATE CKingSpiedr_Run::Update_State(const _float& fTimeDelta)
{
	return STATE();
}

void CKingSpiedr_Run::LateUpdate_State()
{
}

void CKingSpiedr_Run::Render_State()
{
}

CKingSpiedr_Run* CKingSpiedr_Run::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpiedr_Run* pState = new CKingSpiedr_Run(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpiedr_Run*>(pState);

		MSG_BOX("KingSpider_Run Failed");
		return nullptr;
	}

	return pState;
}

void CKingSpiedr_Run::Free()
{
	__super::Free();
}
