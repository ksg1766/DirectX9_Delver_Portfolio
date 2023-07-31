#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpider_ShootPoison.h"
#include "Export_Function.h"
#include "Player.h"

CKingSpider_ShootPoison::CKingSpider_ShootPoison()
{
}

CKingSpider_ShootPoison::CKingSpider_ShootPoison(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpider_ShootPoison::~CKingSpider_ShootPoison()
{
}

HRESULT CKingSpider_ShootPoison::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;


	return S_OK;
}

STATE CKingSpider_ShootPoison::Update_State(const _float& fTimeDelta)
{
	return STATE();
}

void CKingSpider_ShootPoison::LateUpdate_State()
{
}

void CKingSpider_ShootPoison::Render_State()
{
}

CKingSpider_ShootPoison* CKingSpider_ShootPoison::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpider_ShootPoison* pState = new CKingSpider_ShootPoison(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpider_ShootPoison*>(pState);

		MSG_BOX("KingSpider_Jump Failed");
		return nullptr;
	}

	return pState;
}

void CKingSpider_ShootPoison::Free()
{
	__super::Free();
}
