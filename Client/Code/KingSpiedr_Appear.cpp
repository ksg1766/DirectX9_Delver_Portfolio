#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpiedr_Appear.h"
#include "Export_Function.h"
#include "Player.h"

CKingSpiedr_Appear::CKingSpiedr_Appear()
{
}

CKingSpiedr_Appear::CKingSpiedr_Appear(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpiedr_Appear::~CKingSpiedr_Appear()
{
}

HRESULT CKingSpiedr_Appear::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;


	return S_OK;
}

STATE CKingSpiedr_Appear::Update_State(const _float& fTimeDelta)
{
	return STATE();
}

void CKingSpiedr_Appear::LateUpdate_State()
{
}

void CKingSpiedr_Appear::Render_State()
{
}

CKingSpiedr_Appear* CKingSpiedr_Appear::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpiedr_Appear* pState = new CKingSpiedr_Appear(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpiedr_Appear*>(pState);

		MSG_BOX("KingSpider_Run Failed");
		return nullptr;
	}

	return pState;
}

void CKingSpiedr_Appear::Free()
{
	__super::Free();
}
