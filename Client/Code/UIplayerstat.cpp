#include "stdafx.h"
#include "..\Header\UIplayerstat.h"
#include "Player.h"

CUIplayerstat::CUIplayerstat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIplayerstat::~CUIplayerstat()
{

}

HRESULT CUIplayerstat::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_matAttackPowerNumberOne = WorldMatrix(735.f, 560.f, 5.f, 6.f);
	m_matAttackPowerNumberTwo = WorldMatrix(749.f, 560.f, 5.f, 6.f);
	m_matArmorClassNumberOne = WorldMatrix(735.f, 543.f, 5.f, 6.f);
	m_matArmorClassNumberTwo = WorldMatrix(749.f, 543.f, 5.f, 6.f);

	m_matHealthNumberOne = WorldMatrix(608.f, 490.f, 5.f, 6.f);
	m_matHealthNumberTwo = WorldMatrix(622.f, 490.f, 5.f, 6.f);
	m_matMagicNumberOne = WorldMatrix(608.f, 473.f, 5.f, 6.f);
	m_matMagicNumberTwo = WorldMatrix(622.f, 473.f, 5.f, 6.f);
	m_matSpeedNumberOne = WorldMatrix(608.f, 456.f, 5.f, 6.f);
	m_matSpeedNumberTwo = WorldMatrix(622.f, 456.f, 5.f, 6.f);

	m_matAttackNumberOne = WorldMatrix(763.f, 490.f, 5.f, 6.f);
	m_matAttackNumberTwo = WorldMatrix(777.f, 490.f, 5.f, 6.f);
	m_matDefenseNumberOne = WorldMatrix(763.f, 473.f, 5.f, 6.f);
	m_matDefenseNumberTwo = WorldMatrix(777.f, 473.f, 5.f, 6.f);
	m_matAgilityNumberOne = WorldMatrix(763.f, 456.f, 5.f, 6.f);
	m_matAgilityNumberTwo = WorldMatrix(777.f, 456.f, 5.f, 6.f);
    
    m_matLevelNumberOne = WorldMatrix(702.f, 404.f, 5.f, 6.f);
	m_matLevelNumberTwo = WorldMatrix(716.f, 404.f, 5.f, 6.f);

	m_matCurrentXPNumberOne = WorldMatrix(665.f, 387.f, 5.f, 6.f);
	m_matCurrentXPNumberTwo = WorldMatrix(679.f, 387.f, 5.f, 6.f);
	m_matSlash          = WorldMatrix(688.f, 387.f, 5.f, 6.f);
	m_matMaxXPNumberOne = WorldMatrix(702.f, 387.f, 5.f, 6.f);
	m_matMaxXPNumberTwo = WorldMatrix(716.f, 387.f, 5.f, 6.f);

    m_matGoldNumberOne = WorldMatrix(702.f, 370.f, 5.f, 6.f);
	m_matGoldNumberTwo = WorldMatrix(716.f, 370.f, 5.f, 6.f);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = 475.f;
	m_pTransform->m_vLocalScale.x = 180.f;
	m_pTransform->m_vLocalScale.y = 140.f;
	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	m_fAttackPowerNumberMin = 1;
	m_fArmorClassNumberMin = 2;

	m_fHealthNumber = 3;
	m_fMagicNumber = 4;
	m_fSpeedNumber = 5;

	m_fAttackNumber = 6;
	m_fDefenseNumber = 7;
	m_fAgilityNumber = 8;

	m_fLevelNumber = 9;
	m_fCurrentXPNumber = 10;
	m_fMaxXPNumber = 11;
	m_fGoldNumber = 12;

	return S_OK;
}

_int CUIplayerstat::Update_Object(const _float & fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIplayerstat::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();

	CPlayer& rPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	m_fAttackPowerNumberMin = rPlayer.Get_Stat()->Get_Stat()->iDamageMin;
	m_fAttackPowerNumberMax = rPlayer.Get_Stat()->Get_Stat()->iDamageMax;
	m_fArmorClassNumberMin = rPlayer.Get_Stat()->Get_Stat()->iArmorMin;
	m_fArmorClassNumberMax = rPlayer.Get_Stat()->Get_Stat()->iArmorMax;

	m_fHealthNumber		= rPlayer.Get_Stat()->Get_Stat()->fHealth;
	m_fMagicNumber		= rPlayer.Get_Stat()->Get_Stat()->fMagic;
	m_fSpeedNumber		= rPlayer.Get_Stat()->Get_Stat()->fSpeed;

	m_fAttackNumber		= rPlayer.Get_Stat()->Get_Stat()->fAttack;
	m_fDefenseNumber	= rPlayer.Get_Stat()->Get_Stat()->fDeffense;
	m_fAgilityNumber	= rPlayer.Get_Stat()->Get_Stat()->fAgility;

	m_fLevelNumber		= rPlayer.Get_Stat()->Get_Stat()->iLevel;
	m_fCurrentXPNumber	= rPlayer.Get_Stat()->Get_Stat()->iExp;
	m_fMaxXPNumber		= rPlayer.Get_Stat()->Get_Stat()->iExpMax;
	m_fGoldNumber		= rPlayer.Get_Stat()->Get_Stat()->iGold;

	Update_NumverUI();
}

void CUIplayerstat::Render_Object()
{
	if (m_IsDead)
		return;

	// 메인 셋업 UI박스
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	// AttackPower UI
	if (m_fAttackPowerNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matAttackPowerNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fAttackPowerNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matAttackPowerNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fAttackPowerNumberTwo);
	m_pBufferCom->Render_Buffer();

	// ArmorClass UI
	if (m_fArmorClassNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matArmorClassNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fArmorClassNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matArmorClassNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fArmorClassNumberTwo);
	m_pBufferCom->Render_Buffer();

	// Health UI
	if (m_fHealthNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matHealthNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fHealthNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matHealthNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fHealthNumberTwo);
	m_pBufferCom->Render_Buffer();

	// Magic UI
	if (m_fMagicNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matMagicNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fMagicNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matMagicNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fMagicNumberTwo);
	m_pBufferCom->Render_Buffer();

	// Speed UI
	if (m_fSpeedNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matSpeedNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fSpeedNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matSpeedNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fSpeedNumberTwo);
	m_pBufferCom->Render_Buffer();

	// Attack UI
	if (m_fAttackNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matAttackNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fAttackNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matAttackNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fAttackNumberTwo);
	m_pBufferCom->Render_Buffer();

	// Defense UI
	if (m_fAttackNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matDefenseNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fDefenseNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matDefenseNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fDefenseNumberTwo);
	m_pBufferCom->Render_Buffer();

	// Agility UI
	if (m_fAgilityNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matAgilityNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fAgilityNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matAgilityNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fAgilityNumberTwo);
	m_pBufferCom->Render_Buffer();

	// Level UI
	if (m_fLevelNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matLevelNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fLevelNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matLevelNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fLevelNumberTwo);
	m_pBufferCom->Render_Buffer();

	// Current XP UI
	if (m_fCurrentXPNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matCurrentXPNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fCurrentXPNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matCurrentXPNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fCurrentXPNumberTwo);
	m_pBufferCom->Render_Buffer();

	// Slash UI
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matSlash);
	m_pNumberTextureCom->Render_Texture(10);
	m_pBufferCom->Render_Buffer();

	// Max XP UI
	if (m_fMaxXPNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matMaxXPNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fMaxXPNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matMaxXPNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fMaxXPNumberTwo);
	m_pBufferCom->Render_Buffer();

	// Gold UI
	if (m_fGoldNumberOne != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matGoldNumberOne);
		m_pNumberTextureCom->Render_Texture(m_fGoldNumberOne);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matGoldNumberTwo);
	m_pNumberTextureCom->Render_Texture(m_fGoldNumberTwo);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIplayerstat::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTexBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_playerstatUI"));
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

void CUIplayerstat::Key_Input(void)
{
}

void CUIplayerstat::Update_NumverUI()
{
	Check_Number(m_fAttackPowerNumberMin, m_fAttackPowerNumberOne, m_fAttackPowerNumberTwo);
	Check_Number(m_fArmorClassNumberMin, m_fArmorClassNumberOne, m_fArmorClassNumberTwo);
	Check_Number(m_fHealthNumber, m_fHealthNumberOne, m_fHealthNumberTwo);
	Check_Number(m_fMagicNumber, m_fMagicNumberOne, m_fMagicNumberTwo);
	Check_Number(m_fSpeedNumber, m_fSpeedNumberOne, m_fSpeedNumberTwo);
	Check_Number(m_fAttackNumber, m_fAttackNumberOne, m_fAttackNumberTwo);
	Check_Number(m_fDefenseNumber, m_fDefenseNumberOne, m_fDefenseNumberTwo);
	Check_Number(m_fAgilityNumber, m_fAgilityNumberOne, m_fAgilityNumberTwo);
	Check_Number(m_fLevelNumber, m_fLevelNumberOne, m_fLevelNumberTwo);
	Check_Number(m_fCurrentXPNumber, m_fCurrentXPNumberOne, m_fCurrentXPNumberTwo);
	Check_Number(m_fMaxXPNumber, m_fMaxXPNumberOne, m_fMaxXPNumberTwo);
	Check_Number(m_fGoldNumber, m_fGoldNumberOne, m_fGoldNumberTwo);
}

void CUIplayerstat::Check_Number(_uint Number, _uint& NumberOne, _uint& NumberTwo)
{
	_int iCurrentOneNum = Number / 10;
	_int iCurrentTwoNum = Number % 10;

	switch (iCurrentOneNum)
	{
	case 0:
		NumberOne = 0;
		break;
	case 1:
		NumberOne = 1;
		break;
	case 2:
		NumberOne = 2;
		break;
	case 3:
		NumberOne = 3;
		break;
	case 4:
		NumberOne = 4;
		break;
	case 5:
		NumberOne = 5;
		break;
	case 6:
		NumberOne = 6;
		break;
	case 7:
		NumberOne = 7;
		break;
	case 8:
		NumberOne = 8;
		break;
	case 9:
		NumberOne = 9;
		break;
	}

	switch (iCurrentTwoNum)
	{
	case 0:
		NumberTwo = 0;
		break;
	case 1:
		NumberTwo = 1;
		break;
	case 2:
		NumberTwo = 2;
		break;
	case 3:
		NumberTwo = 3;
		break;
	case 4:
		NumberTwo = 4;
		break;
	case 5:
		NumberTwo = 5;
		break;
	case 6:
		NumberTwo = 6;
		break;
	case 7:
		NumberTwo = 7;
		break;
	case 8:
		NumberTwo = 8;
		break;
	case 9:
		NumberTwo = 9;
		break;
	}
}

CUIplayerstat* CUIplayerstat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIplayerstat*	pInstance = new CUIplayerstat(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUIplayerstat::Free()
{
	CTempUI::Free();
}
