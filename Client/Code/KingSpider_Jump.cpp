#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpider_Jump.h"
#include "Export_Function.h"
#include "Player.h"

CKingSpider_Jump::CKingSpider_Jump()
{
}

CKingSpider_Jump::CKingSpider_Jump(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpider_Jump::~CKingSpider_Jump()
{
}

HRESULT CKingSpider_Jump::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;


	return S_OK;
}

STATE CKingSpider_Jump::Update_State(const _float& fTimeDelta)
{
	return STATE();
}

void CKingSpider_Jump::LateUpdate_State()
{
}

void CKingSpider_Jump::Render_State()
{
}

CKingSpider_Jump* CKingSpider_Jump::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpider_Jump* pState = new CKingSpider_Jump(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpider_Jump*>(pState);

		MSG_BOX("KingSpider_Jump Failed");
		return nullptr;
	}

	return pState;
}

void CKingSpider_Jump::Free()
{
	__super::Free();
}
