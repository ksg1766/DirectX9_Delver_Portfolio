#include "stdafx.h"
#include "..\Header\UIitem.h"

CUIitem::CUIitem(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIitem::~CUIitem()
{

}

HRESULT CUIitem::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vLocalScale.x = 23.f;
	m_pTransform->m_vLocalScale.y = 23.f;

	return S_OK;
}

_int CUIitem::Update_Object(const _float & fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIitem::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();

	Update_NumverUI();
}

void CUIitem::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();

	// 숫자 개수 출력
	if (m_ItemID.iCount > 1)
	{
		_matrix      matWorld;

		if (m_iCurrentOneNum != 0)
		{
			D3DXMatrixIdentity(&matWorld);
			matWorld._11 = 4.2f;
			matWorld._22 = 5.1f;
			matWorld._41 = m_matWorld._41 + 8.f;
			matWorld._42 = m_matWorld._42 + 15.f;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNumberTextureCom->Render_Texture(m_iCurrentOneNum);
			m_pBufferCom->Render_Buffer();
		}

		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 4.2f;
		matWorld._22 = 5.1f;
		matWorld._41 = m_matWorld._41 + 17.f;
		matWorld._42 = m_matWorld._42 + 15.f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pNumberTextureCom->Render_Texture(m_iCurrentTwoNum);
		m_pBufferCom->Render_Buffer();
	}

	if (m_bTooltipRender)
	{
		_matrix      matWorld;

		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = m_fTooltipPosX + 41.f;
		matWorld._42 = m_fTooltipPosY + 21.f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pTooltipCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CUIitem::Add_Component(void)
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

	pComponent = m_pNumberTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_NumberUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE1, pComponent);

	pComponent = m_pTooltipCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_tooltipUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE1, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIitem::Key_Input(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y))
	{
		m_bCollider = true;
		// 아이템 UI를 눌렀다가 가져다 놨을 때 해당 마우스 위치의 슬롯이 비어있는지 판별 후 여부에 따른 처리
		if (Engine::InputDev()->Mouse_Pressing(DIM_LB))
		{
			m_bMove = true;
			WorldMatrix(pt.x, pt.y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
		}

		if(!m_bMove)
		{
			m_bTooltipRender = true;

			m_fTooltipPosX = pt.x;
			m_fTooltipPosY = pt.y;
		}

		//if (Engine::InputDev()->Mouse_Up(DIM_LB))
		//{
		//	m_bMove = false;
		//}
	}
	else {
		m_bMove = false;
		m_bCollider = false;
		m_bTooltipRender = false;
	}
}

void CUIitem::Update_NumverUI()
{
	_int iCurrentOneNum = m_ItemID.iCount / 10;
	_int iCurrentTwoNum = m_ItemID.iCount % 10;

	switch (iCurrentOneNum)
	{
	case 0:
		m_iCurrentOneNum = 0;
		break;
	case 1:
		m_iCurrentOneNum = 1;
		break;
	case 2:
		m_iCurrentOneNum = 2;
		break;
	case 3:
		m_iCurrentOneNum = 3;
		break;
	case 4:
		m_iCurrentOneNum = 4;
		break;
	case 5:
		m_iCurrentOneNum = 5;
		break;
	case 6:
		m_iCurrentOneNum = 6;
		break;
	case 7:
		m_iCurrentOneNum = 7;
		break;
	case 8:
		m_iCurrentOneNum = 8;
		break;
	case 9:
		m_iCurrentOneNum = 9;
		break;
	}

	switch (iCurrentTwoNum)
	{
	case 0:
		m_iCurrentTwoNum = 0;
		break;
	case 1:
		m_iCurrentTwoNum = 1;
		break;
	case 2:
		m_iCurrentTwoNum = 2;
		break;
	case 3:
		m_iCurrentTwoNum = 3;
		break;
	case 4:
		m_iCurrentTwoNum = 4;
		break;
	case 5:
		m_iCurrentTwoNum = 5;
		break;
	case 6:
		m_iCurrentTwoNum = 6;
		break;
	case 7:
		m_iCurrentTwoNum = 7;
		break;
	case 8:
		m_iCurrentTwoNum = 8;
		break;
	case 9:
		m_iCurrentTwoNum = 9;
		break;
	}
}

CUIitem* CUIitem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIitem*	pInstance = new CUIitem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUIitem::Free()
{
	CTempUI::Free();
}
