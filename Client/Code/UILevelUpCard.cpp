#include "stdafx.h"
#include "..\Header\UILevelUpCard.h"
#include "Player.h"

CUILevelUpCard::CUILevelUpCard(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUILevelUpCard::~CUILevelUpCard()
{

}

HRESULT CUILevelUpCard::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransform->m_vInfo[INFO_POS].x = 640;
	m_pTransform->m_vInfo[INFO_POS].y = 350;
	m_pTransform->m_vLocalScale.x = 135;
	m_pTransform->m_vLocalScale.y = 275;
	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	return S_OK;
}

_int CUILevelUpCard::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	// 골라진 순서에 따라 해당 위치로 이동
	if (m_bUseCard) {
		m_bUseCard = false;

		// 초기 위치로 이동
	    m_pTransform->m_vInfo[INFO_POS].x = 125.f;
	    m_pTransform->m_vInfo[INFO_POS].y = 35.f;
	    WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

		m_bMoveUpdate = true;
		m_bStatUpdate = true;
	}
	else if (m_bMoveUpdate)
	{
		if (m_iPickNumber == 0) {
			m_bMoveUpdate = false;
		}
		else if (m_iPickNumber == 1 && m_pTransform->m_vInfo[INFO_POS].x < 300.f )
		{
			// 해당 위치까지 이동해라
		}
		else if (m_iPickNumber == 2 && m_pTransform->m_vInfo[INFO_POS].x < 400.f)
		{
			// 해당 위치까지 이동해라
		}
	}

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUILevelUpCard::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	if (m_bStatUpdate) {
		m_bStatUpdate = false;

		// 카드 타입에 따른 현재 스탯 가져와 할당
		switch (m_iTypeImage)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		}
	}

	CTempUI::LateUpdate_Object();

	//CGameObject* pObject = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();
	//if (pObject != nullptr)
	//{
	//	CPlayer& rPlayer = *dynamic_cast<CPlayer*>(pObject);

	//	m_iMaxHp = rPlayer.Get_Stat()->Get_Stat()->fMaxHP;
	//	m_iCurrentHp = rPlayer.Get_Stat()->Get_Stat()->fHP;

	//	Update_NumverUI();
	//}
}

void CUILevelUpCard::Render_Object()
{
	if (m_IsDead)
		return;

	// 첫번째 위치
	m_pTransform->m_vInfo[INFO_POS].x = 340;
	m_pTransform->m_vInfo[INFO_POS].y = 350;
	m_pTransform->m_vLocalScale.x = 135;
	m_pTransform->m_vLocalScale.y = 275;
	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	// 가운데 위치
	m_pTransform->m_vInfo[INFO_POS].x = 640;
	m_pTransform->m_vInfo[INFO_POS].y = 350;
	m_pTransform->m_vLocalScale.x = 135;
	m_pTransform->m_vLocalScale.y = 275;
	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	// 가운데 위치
	m_pTransform->m_vInfo[INFO_POS].x = 940;
	m_pTransform->m_vInfo[INFO_POS].y = 350;
	m_pTransform->m_vLocalScale.x = 135;
	m_pTransform->m_vLocalScale.y = 275;
	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUILevelUpCard::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_LevelUpCard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pNumberTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_LevelUpNumber"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE1, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUILevelUpCard::Key_Input(void)
{
}

//void CUILevelUpCard::Update_NumverUI(void)
//{
//	if (m_iCurrentHp > m_iMaxHp)
//		m_iCurrentHp = m_iMaxHp;
//
//	_int iCurrentOneNum = m_iCurrentHp / 10;
//	_int iCurrentTwoNum = m_iCurrentHp % 10;
//
//	switch (iCurrentOneNum)
//	{
//	case 0:
//		m_iCurrentOneNum = 0;
//		break;
//	case 1:
//		m_iCurrentOneNum = 1;
//		break;
//	case 2:
//		m_iCurrentOneNum = 2;
//		break;
//	case 3:
//		m_iCurrentOneNum = 3;
//		break;
//	case 4:
//		m_iCurrentOneNum = 4;
//		break;
//	case 5:
//		m_iCurrentOneNum = 5;
//		break;
//	case 6:
//		m_iCurrentOneNum = 6;
//		break;
//	case 7:
//		m_iCurrentOneNum = 7;
//		break;
//	case 8:
//		m_iCurrentOneNum = 8;
//		break;
//	case 9:
//		m_iCurrentOneNum = 9;
//		break;
//	}
//
//	switch (iCurrentTwoNum)
//	{
//	case 0:
//		m_iCurrentTwoNum = 0;
//		break;
//	case 1:
//		m_iCurrentTwoNum = 1;
//		break;
//	case 2:
//		m_iCurrentTwoNum = 2;
//		break;
//	case 3:
//		m_iCurrentTwoNum = 3;
//		break;
//	case 4:
//		m_iCurrentTwoNum = 4;
//		break;
//	case 5:
//		m_iCurrentTwoNum = 5;
//		break;
//	case 6:
//		m_iCurrentTwoNum = 6;
//		break;
//	case 7:
//		m_iCurrentTwoNum = 7;
//		break;
//	case 8:
//		m_iCurrentTwoNum = 8;
//		break;
//	case 9:
//		m_iCurrentTwoNum = 9;
//		break;
//	}
//
//	m_flength   = (110.f / m_iMaxHp) * (m_iMaxHp - m_iCurrentHp);
//	m_fPosition = 140.f - (m_flength / 2) - (m_iMaxHp - m_iCurrentHp) * 2.5f;
//}

CUILevelUpCard* CUILevelUpCard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUILevelUpCard* pInstance = new CUILevelUpCard(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CUILevelUpCard Failed");
		return nullptr;
	}

	return pInstance;
}

void CUILevelUpCard::Free()
{
	CTempUI::Free();
}
