#include "stdafx.h"
#include "..\Header\PressFont.h"

CPressFont::CPressFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CPressFont::~CPressFont()
{
}

HRESULT CPressFont::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // ÃÊ±âÈ­

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = 100.f;
	m_pTransform->m_vLocalScale.x = 280;
	m_pTransform->m_vLocalScale.y = 25;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	
	return S_OK;
}

Engine::_int CPressFont::Update_Object(const _float& fTimeDelta)
{
	m_fTime += 70.f * fTimeDelta;

	if (70.f < m_fTime)
	{
		if (m_bRender) {
			m_bRender = false;
		}
		else {
			m_bRender = true;
		}
		m_fTime = 0.f;
	}

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CPressFont::LateUpdate_Object(void)
{
	CTempUI::LateUpdate_Object();
}

void CPressFont::Render_Object(void)
{
	if (m_bRender)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(3);
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CPressFont::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}


void CPressFont::Key_Input(void)
{
}

void CPressFont::Free()
{
	CTempUI::Free();
}

CPressFont* CPressFont::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPressFont*	pInstance = new CPressFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}

	return pInstance;
}
