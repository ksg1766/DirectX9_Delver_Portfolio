#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpider_Shoot.h"
#include "Export_Function.h"
#include "Player.h"

CKingSpider_Shoot::CKingSpider_Shoot()
{
}

CKingSpider_Shoot::CKingSpider_Shoot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpider_Shoot::~CKingSpider_Shoot()
{
}

HRESULT CKingSpider_Shoot::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;


	return S_OK;
}

STATE CKingSpider_Shoot::Update_State(const _float& fTimeDelta)
{
	return STATE();
}

void CKingSpider_Shoot::LateUpdate_State()
{
}

void CKingSpider_Shoot::Render_State()
{
}

CKingSpider_Shoot* CKingSpider_Shoot::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpider_Shoot* pState = new CKingSpider_Shoot(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpider_Shoot*>(pState);

		MSG_BOX("KingSpider_Shoot Failed");
		return nullptr;
	}

	return pState;
}

void CKingSpider_Shoot::Free()
{
	__super::Free();
}
