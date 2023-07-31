#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpider_MakeNest.h"
#include "Export_Function.h"
#include "Player.h"

CKingSpider_MakeNest::CKingSpider_MakeNest()
{
}

CKingSpider_MakeNest::CKingSpider_MakeNest(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpider_MakeNest::~CKingSpider_MakeNest()
{
}

HRESULT CKingSpider_MakeNest::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;


	return S_OK;
}

STATE CKingSpider_MakeNest::Update_State(const _float& fTimeDelta)
{
	return STATE();
}

void CKingSpider_MakeNest::LateUpdate_State()
{
}

void CKingSpider_MakeNest::Render_State()
{
}

CKingSpider_MakeNest* CKingSpider_MakeNest::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpider_MakeNest* pState = new CKingSpider_MakeNest(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpider_MakeNest*>(pState);

		MSG_BOX("KingSpider_Jump Failed");
		return nullptr;
	}

	return pState;
}

void CKingSpider_MakeNest::Free()
{
	__super::Free();
}
