#include "stdafx.h"
#include "..\Header\ShopItem.h"
#include "Export_Function.h"

CShopItem::CShopItem(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CShopItem::~CShopItem()
{
}

HRESULT CShopItem::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vLocalScale.x = 30.f;
	m_pTransform->m_vLocalScale.y = 30.f;
	m_iCollisionNumber = 0;

	return S_OK;
}

_int CShopItem::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CShopItem::LateUpdate_Object()
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CShopItem::Render_Object()
{
	if (m_IsDead)
		return;

	_matrix      matWorld;
#pragma region 장비아이템
	if (m_fCurrentImage == 30)
	{
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = 330;
		matWorld._42 = 500;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fCurrentImage == 33)
	{
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = 330;
		matWorld._42 = 430;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fCurrentImage == 36)
	{
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = 330;
		matWorld._42 = 360;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fCurrentImage == 31)
	{
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = 330;
		matWorld._42 = 290;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fCurrentImage == 17)
	{
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = 330;
		matWorld._42 = 220;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fCurrentImage == 6)
	{
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = 700;
		matWorld._42 = 500;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fCurrentImage == 7)
	{
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = 700;
		matWorld._42 = 430;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fCurrentImage == 8)
	{
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = 700;
		matWorld._42 = 360;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fCurrentImage == 24)
	{
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = 700;
		matWorld._42 = 290;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fCurrentImage == 29)
	{
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = 700;
		matWorld._42 = 220;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}

#pragma endregion 장비아이템

#pragma region 폰트

	if (m_iCollisionNumber == 0)
	{
		if (m_iCurrentFont == 1)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 510;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 495;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFont);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFont == 3)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 510;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 430;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFont);
			m_pBufferCom->Render_Buffer();
		}
	}
	else if(m_iCollisionNumber == 1)
	{
		if (m_iCurrentFontAlpha == 0)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 510;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 495;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFontAlpha);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFontAlpha == 2)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 510;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 430;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFontAlpha);
			m_pBufferCom->Render_Buffer();
		}
	}

#pragma endregion 폰트

}

void CShopItem::Key_Input()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, 
		m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x,
		m_pTransform->m_vLocalScale.y))
		m_iCollisionNumber = 1;
	else
		m_iCollisionNumber = 0;
}

HRESULT CShopItem::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTexBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_itemUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pNameTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_ShopFont"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CShopItem* CShopItem::Create(LPDIRECT3DDEVICE9 pGrahpicDev)
{
	CShopItem* pInstance = new CShopItem(pGrahpicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CShopItem*>(pInstance);

		MSG_BOX("Create ItemUI Failed");
		return nullptr;
	}

	return pInstance;
}

void CShopItem::Free()
{
	__super::Free();
}
