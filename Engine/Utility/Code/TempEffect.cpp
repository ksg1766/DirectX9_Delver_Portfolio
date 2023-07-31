#include "..\..\Header\TempEffect.h"

CTempEffect::CTempEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTempEffect::CTempEffect(const CTempEffect& rhs)
	: CGameObject(rhs.m_pGraphicDev)//, ~~~~
{
}

CTempEffect::~CTempEffect()
{
	
}

HRESULT CTempEffect::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::EFFECT;

	m_pBufferCom = nullptr;
	m_pTextureCom = nullptr;
	m_pBillBoardCom = nullptr;

	m_fTime = 0.f;

	m_fFrame = 0.f;
	m_fFrist = 0.f;
	m_fFinal = 0.f;
	m_fFrameSpeed = 0.f;

	m_fLife = 0.f;

	m_bAnimation = false;
	m_bLoop      = false;
	m_bAniEnd    = false;

	m_RandomSet  = true;
	m_bScaleSet  = false;
	m_bCreateSet = false;

	m_bParent = false;
	m_bChild  = false;

	return S_OK;
}

_int CTempEffect::Update_Object(const _float & fTimeDelta)
{
	m_fTime += fTimeDelta;

	/*if (m_fTime > m_fLife || m_fFrame == m_fFinal && m_bAnimation && !m_bLoop) {
		m_IsDead = true;
		Engine::EventManager()->DeleteObject(this);
	}*/

	if (m_bAnimation)
	{
		m_fFrame += m_fFinal * fTimeDelta * m_fFrameSpeed;

		if (m_fFinal < m_fFrame)
		{
			if (m_bLoop) {
				m_bAniEnd = true;
				m_fFrame = m_fFrist;
			}
			else {
				m_fFrame = m_fFinal;
			}
		}
	}
	
	//_int iExit = CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CTempEffect::LateUpdate_Object(void)
{
	// CGameObject::LateUpdate_Object();
}

void CTempEffect::Render_Object(void)
{
}

void CTempEffect::Free(void)
{
	CGameObject::Free();
}
