#include "stdafx.h"
#include "..\Header\WhiteOutIn.h"
#include "GameManager.h"

CWhiteOutIn::CWhiteOutIn(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CWhiteOutIn::~CWhiteOutIn()
{
}

HRESULT CWhiteOutIn::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY / 2;
	m_pTransform->m_vLocalScale.x = 650;
	m_pTransform->m_vLocalScale.y = 370;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	
	m_fFrame = 37.f;

	return S_OK;
}

Engine::_int CWhiteOutIn::Update_Object(const _float& fTimeDelta)
{
	//_int iExit = CTempUI::Update_Object(fTimeDelta);
	if (m_IsDead)
		return 0;

	m_fPlayModeTime += 1.f * fTimeDelta;
	m_fFrame -= 37.f * fTimeDelta;

	if (19.f >= m_fFrame) {
		m_bMiddle = true;
	}

	if (0.f >= m_fFrame) {
		m_fFrame = 0.f;
	}

	if (m_fPlayModeTime > 1.5f) {
		// 엔딩 연출 재생
		CGameManager::GetInstance()->PlayMode(PD::ClearGame);

		// 엔딩 연출 끝날 시 엔딩 크래딧 씬으로 전환

		m_IsDead = true;
	}

	return 0;
}

void CWhiteOutIn::LateUpdate_Object(void)
{
	CTempUI::LateUpdate_Object();
}

void CWhiteOutIn::Render_Object(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((int)m_fFrame, 255, 255, 255));
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CWhiteOutIn::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_UIWhite"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}


void CWhiteOutIn::Key_Input(void)
{
}

void CWhiteOutIn::Free()
{
	CTempUI::Free();
}

CWhiteOutIn* CWhiteOutIn::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWhiteOutIn*	pInstance = new CWhiteOutIn(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CWhiteOutIn Create Failed");
		return nullptr;
	}

	return pInstance;
}
