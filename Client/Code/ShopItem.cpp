#include "stdafx.h"
#include "..\Header\ShopItem.h"
#include "Export_Function.h"
#include "Itemgroup.h"
#include "Player.h"
#include "SoundManager.h"
#include "UIitem.h"
#include <UIbasicslot.h>
#include <UIemptyslot.h>

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
	m_iBuyCount = 1;
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

	if (m_fCurrentImage == 38)
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

		if (m_iCurrentFont == 5)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 510;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 360;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFont);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFont == 7)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 510;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 290;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFont);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFont == 9)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 510;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 220;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFont);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFont == 11)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 870;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 500;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFont);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFont == 13)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 870;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 430;		

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFont);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFont == 15)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 870;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 360;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFont);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFont == 17)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 870;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 290;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFont);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFont == 19)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 870;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 220;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

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

		if (m_iCurrentFontAlpha == 4)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 510;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 360;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFontAlpha);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFontAlpha == 6)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 510;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 290;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFontAlpha);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFontAlpha == 8)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 510;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 220;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFontAlpha);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFontAlpha == 10)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 870;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 500;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFontAlpha);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFontAlpha == 12)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 870;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 430;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFontAlpha);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFontAlpha == 14)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 870;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 360;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFontAlpha);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFontAlpha == 16)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 870;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 290;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pNameTexture->Render_Texture(m_iCurrentFontAlpha);
			m_pBufferCom->Render_Buffer();
		}

		if (m_iCurrentFontAlpha == 18)
		{
			D3DXMatrixIdentity(&matWorld);
			m_pTransform->m_vLocalScale.x = matWorld._11 = 88.f;
			m_pTransform->m_vLocalScale.y = matWorld._22 = 36.f;
			m_pTransform->m_vInfo[INFO_POS].x = matWorld._41 = 870;
			m_pTransform->m_vInfo[INFO_POS].y = matWorld._42 = 220;

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
	{
		m_iCollisionNumber = 1;

		if (Engine::InputDev()->Mouse_Down(DIM_LB) && m_iBuyCount != 0)
		{
			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
			CSoundManager::GetInstance()->PlaySound(L"ui_buy.mp3", CHANNELID::SOUND_UI, 1.f);

			CGameObject* pItem = nullptr;
			CPlayer& rPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

			switch (m_ItemID.eItemID)
			{
			case WEAPON_SWORD:
				pItem = CTempItem::Create(m_pGraphicDev, false);
				--m_iBuyCount;
				break;
			case WEAPON_BOW:
				pItem = CBow::Create(m_pGraphicDev, false);
				--m_iBuyCount;
				break;
			case WEAPON_WAND3:
				pItem = CFireWands::Create(m_pGraphicDev, false);
				--m_iBuyCount;
				break;
			case GENERAL_SHIELD:
				pItem = CShield::Create(m_pGraphicDev, false);
				--m_iBuyCount;
				break;
			case GENERAL_LAMP:
				pItem = CLamp::Create(m_pGraphicDev, false);
				--m_iBuyCount;
				break;
			case EQUIP_OLDHELMET:
				pItem = CHelmet::Create(m_pGraphicDev, false);
				--m_iBuyCount;
				break;
			case EQUIP_OLDARMOR:
				pItem = CTop::Create(m_pGraphicDev, false);
				--m_iBuyCount;
				break;
			case EQUIP_OLDTROUSERS:
				pItem = CPants::Create(m_pGraphicDev, false);
				--m_iBuyCount;
				break;
			case EAT_POTION2:
				pItem = CHpPotion::Create(m_pGraphicDev, false);
				--m_iBuyCount;
				break;
			case EAT_POTION7:
				pItem = CHolyWater::Create(m_pGraphicDev, false);
				--m_iBuyCount;
				break;
			}

			ITEMTYPEID ItemType = dynamic_cast<CItem*>(pItem)->Get_ItemTag();

			rPlayer.Get_Inventory()->Add_ItemObject(pItem);

			Engine::CGameObject* pGameObjectUI = CUIitem::Create(m_pGraphicDev);
			dynamic_cast<CUIitem*>(pGameObjectUI)->Set_ItemTag(ItemType.eItemType, ItemType.eItemID, ItemType.iCount);

			if (ItemType.eItemType == ITEMTYPE_WEAPONITEM && !rPlayer.Get_ItemEquipRight())
			{
				rPlayer.Set_ItemEquipRight(true);

				rPlayer.Set_CurrentEquipRight(pItem);
				rPlayer.Set_PrevEquipRight(pItem);
				Engine::EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);

				Engine::UIManager()->AddItemGameobject_UI(pGameObjectUI);

				Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);
			}
//			else if (ItemType.eItemType == ITEMTYPE::ITEMTYPE_GENERALITEM && !rPlayer.Get_ItemEquipLeft())
//			{
//				rPlayer.Set_ItemEquipLeft(true);
//
//				rPlayer.Set_CurrentEquipLeft(pItem);
//				rPlayer.Set_PrevEquipLeft(pItem);
//				Engine::EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);
//
//				_uint ItemSlotNumber = 0;
//				switch (ItemType.eItemID)
//				{
//				case GENERAL_SHIELD:
//					ItemSlotNumber = 1;
//					break;
//				case GENERAL_LAMP:
//					ItemSlotNumber = 1;
//					break;
//				case GENERAL_BEER:
//					ItemSlotNumber = 1;
//					break;
//				case EQUIP_OLDHELMET:
//					ItemSlotNumber = 0;
//					break;
//				case EQUIP_OLDARMOR:
//					ItemSlotNumber = 2;
//					break;
//				case EQUIP_OLDTROUSERS:
//					ItemSlotNumber = 4;
//					break;
//				case EQUIP_IRONHELMET:
//					ItemSlotNumber = 0;
//					break;
//				case EQUIP_IRONARMOR:
//					ItemSlotNumber = 2;
//					break;
//				case EQUIP_IRONTROUSERS:
//					ItemSlotNumber = 4;
//					break;
//				case EQUIP_SMALLSILVERRING:
//					ItemSlotNumber = 3;
//					break;
//				case EQUIP_BIGSILVERRING:
//					ItemSlotNumber = 3;
//					break;
//				case EQUIP_SILVERNECKLACE:
//					ItemSlotNumber = 5;
//					break;
//				case EQUIP_SMALLGOLDRING:
//					ItemSlotNumber = 3;
//					break;
//				case EQUIP_BIGGOLDRING:
//					ItemSlotNumber = 3;
//					break;
//				case EQUIP_GOLDNECKLACE:
//					ItemSlotNumber = 5;
//					break;
//				}
//
//				CGameObject* SlotObject = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIID_SLOTEQUIPMENT, ItemSlotNumber);
//
//				if (dynamic_cast<CTempUI*>(SlotObject)->Get_EmptyBool()) // 해당 슬롯이 비어있으면
//				{
//					// 들어온 아이템 ui 포지션을 비어있는 해당 슬롯 포지션 값으로 대입 후 월드 행렬 셋팅
//					pGameObjectUI->m_pTransform->m_vInfo[INFO_POS].x = SlotObject->m_pTransform->m_vInfo[INFO_POS].x;
//					pGameObjectUI->m_pTransform->m_vInfo[INFO_POS].y = SlotObject->m_pTransform->m_vInfo[INFO_POS].y;
//					dynamic_cast<CTempUI*>(pGameObjectUI)->WorldMatrix(pGameObjectUI->m_pTransform->m_vInfo[INFO_POS].x, pGameObjectUI->m_pTransform->m_vInfo[INFO_POS].y, pGameObjectUI->m_pTransform->m_vLocalScale.x, pGameObjectUI->m_pTransform->m_vLocalScale.y);
//
//					// 아이템의 부모 오브젝트로 해당 슬롯 등록
//					dynamic_cast<CTempUI*>(pGameObjectUI)->Set_Parent(SlotObject);
//					// 슬롯 자식 오브젝트로 해당 아이템 등록 후 비어있지 않다는 상태로 변경
//					dynamic_cast<CTempUI*>(SlotObject)->Set_Child(pGameObjectUI);
//					dynamic_cast<CTempUI*>(SlotObject)->Set_EmptyBool(false);
//
//					Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_ITEM, Engine::UILAYER::UI_DOWN, pGameObjectUI);
//					Engine::UIManager()->Hide_InvenItem(0);
//				}
//				else // 비어 있지 않으면 그냥 추가
//				{
//					Engine::UIManager()->AddItemGameobject_UI(pGameObjectUI);
//				}
//			}
			else
			{
				Engine::EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);
				Engine::UIManager()->AddItemGameobject_UI(pGameObjectUI);
			}
		}
	}
	else
	{
		m_iCollisionNumber = 0;
	}
}

HRESULT CShopItem::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
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
