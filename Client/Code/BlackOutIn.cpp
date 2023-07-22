#include "stdafx.h"
#include "..\Header\BlackOutIn.h"

CBlackOutIn::CBlackOutIn(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CBlackOutIn::~CBlackOutIn()
{
}

HRESULT CBlackOutIn::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // ÃÊ±âÈ­

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY / 2;
	m_pTransform->m_vLocalScale.x = 650;
	m_pTransform->m_vLocalScale.y = 370;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	
	m_fFrame = 37.f;

	return S_OK;
}

Engine::_int CBlackOutIn::Update_Object(const _float& fTimeDelta)
{
	//_int iExit = CTempUI::Update_Object(fTimeDelta);

	m_fFrame -= 37.f * fTimeDelta * 2.f;

	if (19.f >= m_fFrame) {
		m_IsDead = true;
	}

	if (0.f >= m_fFrame) {
		m_fFrame = 0.f;
	}

	return 0;
}

void CBlackOutIn::LateUpdate_Object(void)
{
	CTempUI::LateUpdate_Object();
}

void CBlackOutIn::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CBlackOutIn::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_FadeOut"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}


void CBlackOutIn::Key_Input(void)
{
}

void CBlackOutIn::Free()
{
	CTempUI::Free();
}

CBlackOutIn* CBlackOutIn::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBlackOutIn*	pInstance = new CBlackOutIn(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}

	return pInstance;
}
