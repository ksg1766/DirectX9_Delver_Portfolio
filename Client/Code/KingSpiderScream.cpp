#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpiderScream.h"
#include "KingSpider.h"
#include "FlyingCamera.h"
#include "CameraManager.h"


CKingSpiderScream::CKingSpiderScream()
{
}

CKingSpiderScream::CKingSpiderScream(LPDIRECT3DDEVICE9 pGraphicDev)
{
}

CKingSpiderScream::~CKingSpiderScream()
{
}

HRESULT CKingSpiderScream::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_bScream = false;
	m_bSound = false;
	m_fDelay = 0.f;

	return S_OK;
}

STATE CKingSpiderScream::Update_State(const _float& fTimeDelta)
{
	m_fDelay += fTimeDelta;

	if ((1.5f < m_fDelay)&&(!m_bSound))
	{
		//소리추가
		m_bSound = true;
		m_fDelay = 0.f;
	}
	if ((2.5f < m_fDelay) && (m_bSound))
		return STATE::BOSS_IDLE;
}

void CKingSpiderScream::LateUpdate_State()
{
}

void CKingSpiderScream::Render_State()
{
}

CKingSpiderScream* CKingSpiderScream::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	return nullptr;
}

void CKingSpiderScream::Free()
{
}
