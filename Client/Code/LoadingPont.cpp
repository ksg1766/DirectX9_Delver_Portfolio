#include "stdafx.h"
#include "..\Header\LoadingPont.h"

CLoadingPont::CLoadingPont(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CLoadingPont::~CLoadingPont()
{
}

HRESULT CLoadingPont::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BACKGROUND;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // ÃÊ±âÈ­

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = 900;
	m_pTransform->m_vInfo[INFO_POS].y = 145;
	m_pTransform->m_vLocalScale.x = 105;
	m_pTransform->m_vLocalScale.y = 25;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	
	return S_OK;
}

Engine::_int CLoadingPont::Update_Object(const _float& fTimeDelta)
{
	m_fFrame += 11.f * fTimeDelta;

	if (11.f < m_fFrame)
		m_fFrame = 0.f;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CLoadingPont::LateUpdate_Object(void)
{
	CTempUI::LateUpdate_Object();
}

void CLoadingPont::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CLoadingPont::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_StageLoadingFont"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}


void CLoadingPont::Key_Input(void)
{
}

void CLoadingPont::Free()
{
	CTempUI::Free();
}

CLoadingPont* CLoadingPont::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLoadingPont*	pInstance = new CLoadingPont(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}

	return pInstance;
}
