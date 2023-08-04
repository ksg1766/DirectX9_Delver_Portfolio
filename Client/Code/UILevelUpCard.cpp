#include "stdafx.h"
#include "..\Header\UILevelUpCard.h"
#include "Player.h"
#include "UILevelUp.h"

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

	m_pTransform->m_vLocalScale.x = 135;
	m_pTransform->m_vLocalScale.y = 275;

	return S_OK;
}

_int CUILevelUpCard::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;
	
	if (m_bUseCard) {
		m_bUseCard = false;

		// 초기 위치 첫 번째 위치로 이동
		m_pTransform->m_vInfo[INFO_POS].x = 370;
		m_pTransform->m_vInfo[INFO_POS].y = 350;
	    WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

		m_bMoveUpdate = true;
		m_bStatUpdate = true;
	}

	// 골라진 순서에 따라 해당 위치로 이동
	if (m_bMoveUpdate) {
		if (m_iPickNumber == 0 && m_fFirstCardMoveTime < 2.f) {
			m_fFirstCardMoveTime += 1.f * fTimeDelta;
		}
		else if (m_iPickNumber == 1 && m_pTransform->m_vInfo[INFO_POS].x < 640.f) {
			m_pTransform->m_vInfo[INFO_POS].x += 150.f * fTimeDelta;
			WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
		}
		else if (m_iPickNumber == 2 && m_pTransform->m_vInfo[INFO_POS].x < 910.f) {
			m_pTransform->m_vInfo[INFO_POS].x += 300.f * fTimeDelta;
			WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
		}
		else
			m_bMoveUpdate = false;
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

		// 카드 타입에 따른 현재 스탯 가져와 할당 (현재 스탯 보여주기 용도)
		switch (m_iTypeImage)
		{
		case 0: // AGILITY
			m_iCurrentStatNumber = SceneManager()->Get_Scene()->Get_MainPlayer()->Get_Stat()->Get_Stat()->fAgility;;
			break;

		case 1: // ATTACK 
			m_iCurrentStatNumber = SceneManager()->Get_Scene()->Get_MainPlayer()->Get_Stat()->Get_Stat()->fAttack;
			break;

		case 2: // DEFFENSE 
			m_iCurrentStatNumber = SceneManager()->Get_Scene()->Get_MainPlayer()->Get_Stat()->Get_Stat()->fDeffense;
			break;

		case 3: // HEALTH 
			m_iCurrentStatNumber = SceneManager()->Get_Scene()->Get_MainPlayer()->Get_Stat()->Get_Stat()->fHealth;
			break;

		case 4: // MAGIC 
			m_iCurrentStatNumber = SceneManager()->Get_Scene()->Get_MainPlayer()->Get_Stat()->Get_Stat()->fMagic;
			break;

		case 5: // SPEED
			m_iCurrentStatNumber = SceneManager()->Get_Scene()->Get_MainPlayer()->Get_Stat()->Get_Stat()->fSpeed;
			break;
		}

		Update_NumberUI();
	}

	CTempUI::LateUpdate_Object();

	//CGameObject* pObject = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();
	//if (pObject != nullptr)
	//{
	//	CPlayer& rPlayer = *static_cast<CPlayer*>(pObject);

	//	m_iMaxHp = rPlayer.Get_Stat()->Get_Stat()->fMaxHP;
	//	m_iCurrentHp = rPlayer.Get_Stat()->Get_Stat()->fHP;

	//	Update_NumverUI();
	//}
}

void CUILevelUpCard::Render_Object()
{
	if (m_IsDead)
		return;

	if (m_bMouseCollision)
	{
		m_pTransform->m_vLocalScale.x = 143;
		m_pTransform->m_vLocalScale.y = 283;
		WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_iTypeImage);
		m_pBufferCom->Render_Buffer();

		// 현재 카드에 해당하는 스탯 넘버 출력 // 첫 번째 숫자
		_matrix      matWorld;

		if (m_iStatNumberOne != 0) {
			D3DXMatrixIdentity(&matWorld);
			matWorld._11 = 9.f;
			matWorld._22 = 9.f;
			matWorld._41 = m_matWorld._41 + 51.f;
			matWorld._42 = m_matWorld._42 + 124.f;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNumberTexture->Render_Texture(m_iStatNumberOne);
			m_pBufferCom->Render_Buffer();

			//// 두 번째 숫자
			D3DXMatrixIdentity(&matWorld);
			matWorld._11 = 9.f;
			matWorld._22 = 9.f;
			matWorld._41 = m_matWorld._41 + 64.f;
			matWorld._42 = m_matWorld._42 + 124.f;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNumberTexture->Render_Texture(m_iStatNumberTwo);
			m_pBufferCom->Render_Buffer();
		}
		else
		{
			//// 두 번째 숫자
			D3DXMatrixIdentity(&matWorld);
			matWorld._11 = 9.f;
			matWorld._22 = 9.f;
			matWorld._41 = m_matWorld._41 + 51.f;
			matWorld._42 = m_matWorld._42 + 124.f;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNumberTexture->Render_Texture(m_iStatNumberTwo);
			m_pBufferCom->Render_Buffer();
		}
	}
	else
	{
		m_pTransform->m_vLocalScale.x = 135;
		m_pTransform->m_vLocalScale.y = 275;
		WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pTextureCom->Render_Texture(m_iTypeImage);
		m_pBufferCom->Render_Buffer();

		// 현재 카드에 해당하는 스탯 넘버 출력 // 첫 번째 숫자
		_matrix      matWorld;

		if (m_iStatNumberOne != 0) {
			D3DXMatrixIdentity(&matWorld);
			matWorld._11 = 9.f;
			matWorld._22 = 9.f;
			matWorld._41 = m_matWorld._41 + 48.f;
			matWorld._42 = m_matWorld._42 + 120.f;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNumberTexture->Render_Texture(m_iStatNumberOne);
			m_pBufferCom->Render_Buffer();

			//// 두 번째 숫자
			D3DXMatrixIdentity(&matWorld);
			matWorld._11 = 9.f;
			matWorld._22 = 9.f;
			matWorld._41 = m_matWorld._41 + 61.f;
			matWorld._42 = m_matWorld._42 + 120.f;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNumberTexture->Render_Texture(m_iStatNumberTwo);
			m_pBufferCom->Render_Buffer();
		}
		else
		{
			//// 두 번째 숫자
			D3DXMatrixIdentity(&matWorld);
			matWorld._11 = 9.f;
			matWorld._22 = 9.f;
			matWorld._41 = m_matWorld._41 + 48.f;
			matWorld._42 = m_matWorld._42 + 120.f;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNumberTexture->Render_Texture(m_iStatNumberTwo);
			m_pBufferCom->Render_Buffer();
		}
	}
}

HRESULT CUILevelUpCard::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_LevelUpCard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pNumberTexture = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_LevelUpNumber"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE1, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUILevelUpCard::Key_Input(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	if (!m_bMoveUpdate && OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y)) {
		if (Engine::InputDev()->Mouse_Pressing(DIM_LB)) {
			// 해당 카드에 따른 스탯 +1 증가
			switch (m_iTypeImage)
			{
			case 0: // AGILITY
				++rPlayer.Get_Stat()->Get_Stat()->fAgility;
				break;

			case 1: // ATTACK 
				++rPlayer.Get_Stat()->Get_Stat()->fAttack;
				break;

			case 2: // DEFFENSE 
				++rPlayer.Get_Stat()->Get_Stat()->fDeffense;
				break;

			case 3: // HEALTH 
				++rPlayer.Get_Stat()->Get_Stat()->fHealth;
				SceneManager()->Get_Scene()->Get_MainPlayer()->Get_Stat()->Get_Stat()->fMaxHP += 4;
				rPlayer.Get_Stat()->Get_Stat()->fHP = rPlayer.Get_Stat()->Get_Stat()->fMaxHP;
				break;
			case 4: // MAGIC 
				++rPlayer.Get_Stat()->Get_Stat()->fMagic;
				break;

			case 5: // SPEED
				++rPlayer.Get_Stat()->Get_Stat()->fSpeed;
				break;
			}

			// 부모 메인 창 주소를 통한 창 비활성화
			if (m_pParent != nullptr) {
				static_cast<CUILevelUp*>(m_pParent)->Set_NotUseLevelUpUI(true);
			}
		}

		m_bMouseCollision = true;
	}
	else
	{
		m_bMouseCollision = false;
	}
}

void CUILevelUpCard::Update_NumberUI()
{
	_int iCurrentOneNum = m_iCurrentStatNumber / 10;
	_int iCurrentTwoNum = m_iCurrentStatNumber % 10;

	switch (iCurrentOneNum)
	{
	case 0:
		m_iStatNumberOne = 0;
		break;
	case 1:
		m_iStatNumberOne = 1;
		break;
	case 2:
		m_iStatNumberOne = 2;
		break;
	case 3:
		m_iStatNumberOne = 3;
		break;
	case 4:
		m_iStatNumberOne = 4;
		break;
	case 5:
		m_iStatNumberOne = 5;
		break;
	case 6:
		m_iStatNumberOne = 6;
		break;
	case 7:
		m_iStatNumberOne = 7;
		break;
	case 8:
		m_iStatNumberOne = 8;
		break;
	case 9:
		m_iStatNumberOne = 9;
		break;
	}

	switch (iCurrentTwoNum)
	{
	case 0:
		m_iStatNumberTwo = 0;
		break;
	case 1:
		m_iStatNumberTwo = 1;
		break;
	case 2:
		m_iStatNumberTwo = 2;
		break;
	case 3:
		m_iStatNumberTwo = 3;
		break;
	case 4:
		m_iStatNumberTwo = 4;
		break;
	case 5:
		m_iStatNumberTwo = 5;
		break;
	case 6:
		m_iStatNumberTwo = 6;
		break;
	case 7:
		m_iStatNumberTwo = 7;
		break;
	case 8:
		m_iStatNumberTwo = 8;
		break;
	case 9:
		m_iStatNumberTwo = 9;
		break;
	}
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
