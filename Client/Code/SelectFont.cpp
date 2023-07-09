#include "stdafx.h"
#include "..\Header\SelectFont.h"

CSelectFont::CSelectFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CSelectFont::~CSelectFont()
{
}

HRESULT CSelectFont::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BACKGROUND;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // ÃÊ±âÈ­

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = 480.f;
	m_pTransform->m_vLocalScale.x = 180;
	m_pTransform->m_vLocalScale.y = 20;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	
	return S_OK;
}

Engine::_int CSelectFont::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CSelectFont::LateUpdate_Object(void)
{
	CTempUI::LateUpdate_Object();
}

void CSelectFont::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(10);
	m_pBufferCom->Render_Buffer();
}

HRESULT CSelectFont::Add_Component(void)
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


void CSelectFont::Key_Input(void)
{
}

void CSelectFont::Free()
{
	CTempUI::Free();
}

CSelectFont* CSelectFont::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSelectFont*	pInstance = new CSelectFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}

	return pInstance;
}
