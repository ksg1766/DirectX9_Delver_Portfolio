#include "stdafx.h"
#include "..\Header\UIplayerhp.h"

CUIplayerhp::CUIplayerhp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIplayerhp::~CUIplayerhp()
{

}

HRESULT CUIplayerhp::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화

	m_pNumberTextureCom = nullptr;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = 125.f;
	m_pTransform->m_vInfo[INFO_POS].y = 35.f;
	m_pTransform->m_vLocalScale.x = 100.f;
	m_pTransform->m_vLocalScale.y = 25.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	m_iMaxHp = 12;
	m_iCurrentHp = 12;
	m_iCurrentOneNum = 1;
	m_iCurrentTwoNum = 2;

	return S_OK;
}

_int CUIplayerhp::Update_Object(const _float& fTimeDelta)
{
	if (Engine::InputDev()->Key_Down(DIK_9))
		m_iCurrentHp -= 1;
	else if (Engine::InputDev()->Key_Down(DIK_0))
		m_iCurrentHp += 1;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIplayerhp::LateUpdate_Object(void)
{
	CTempUI::LateUpdate_Object();

	Update_NumverUI();
}

void CUIplayerhp::Render_Object()
{
	// 내부 이미지
	m_pTransform->m_vInfo[INFO_POS].x = m_fPosition; // 위치값도 뺀다.
	m_pTransform->m_vInfo[INFO_POS].y = 35.f;
	m_pTransform->m_vLocalScale.x = 110.f - m_flength; // 줄어든만큼
	m_pTransform->m_vLocalScale.y = 25.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(2);
	m_pBufferCom->Render_Buffer();

	// 외부 이미지
	m_pTransform->m_vInfo[INFO_POS].x = 140.f;
	m_pTransform->m_vInfo[INFO_POS].y = 35.f;
	m_pTransform->m_vLocalScale.x = 110.f;
	m_pTransform->m_vLocalScale.y = 25.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();


	// 숫자 이미지
	// 일의 자리
	m_pTransform->m_vInfo[INFO_POS].x = 110.f;
	m_pTransform->m_vInfo[INFO_POS].y = 39.f;
	m_pTransform->m_vLocalScale.x = 7.f;
	m_pTransform->m_vLocalScale.y = 11.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pNumberTextureCom->Render_Texture(m_iCurrentOneNum);
	m_pBufferCom->Render_Buffer();

	// 십의 자리
	m_pTransform->m_vInfo[INFO_POS].x = 125.f;
	m_pTransform->m_vInfo[INFO_POS].y = 39.f;
	m_pTransform->m_vLocalScale.x = 7.f;
	m_pTransform->m_vLocalScale.y = 11.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pNumberTextureCom->Render_Texture(m_iCurrentTwoNum);
	m_pBufferCom->Render_Buffer();

	//
	m_pTransform->m_vInfo[INFO_POS].x = 138.f;
	m_pTransform->m_vInfo[INFO_POS].y = 39.f;
	m_pTransform->m_vLocalScale.x = 7.f;
	m_pTransform->m_vLocalScale.y = 11.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pNumberTextureCom->Render_Texture(10);
	m_pBufferCom->Render_Buffer();

	// 일의 자리
	m_pTransform->m_vInfo[INFO_POS].x = 153.f;
	m_pTransform->m_vInfo[INFO_POS].y = 39.f;
	m_pTransform->m_vLocalScale.x = 7.f;
	m_pTransform->m_vLocalScale.y = 11.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pNumberTextureCom->Render_Texture(1);
	m_pBufferCom->Render_Buffer();

	// 십의 자리
	m_pTransform->m_vInfo[INFO_POS].x = 168.f;
	m_pTransform->m_vInfo[INFO_POS].y = 39.f;
	m_pTransform->m_vLocalScale.x = 7.f;
	m_pTransform->m_vLocalScale.y = 11.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pNumberTextureCom->Render_Texture(2);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIplayerhp::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTexBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_HpBarUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pNumberTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_NumberUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE1, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIplayerhp::Key_Input(void)
{
}

void CUIplayerhp::Update_NumverUI(void)
{
	if (m_iCurrentHp > m_iMaxHp)
		m_iCurrentHp = m_iMaxHp;

	_int iCurrentOneNum = m_iCurrentHp / 10;
	_int iCurrentTwoNum = m_iCurrentHp % 10;

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

	m_flength = (110.f / m_iMaxHp) * (m_iMaxHp - m_iCurrentHp);
	m_fPosition = 140.f - (m_flength / 2) - (m_iMaxHp - m_iCurrentHp) * 2.5f;
}

CUIplayerhp* CUIplayerhp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIplayerhp* pInstance = new CUIplayerhp(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUIplayerhp::Free()
{
	CTempUI::Free();
}
