#pragma once
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CShopItem : public CTempUI
{
private:
	explicit CShopItem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShopItem();

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object();
	void		Render_Object();

	void		Set_ItemTag(ITEMID _itemID)
	{
		m_ItemID.eItemID = _itemID;

		switch (m_ItemID.eItemID)
		{
		case WEAPON_SWORD:
			m_fCurrentImage = 30;
			break;
		case WEAPON_BOW:
			m_fCurrentImage = 33;
			break;
		case WEAPON_WAND3:
			m_fCurrentImage = 38;
			break;
		case GENERAL_SHIELD:
			m_fCurrentImage = 31;
			break;
		case GENERAL_LAMP:
			m_fCurrentImage = 17;
			break;
		case EQUIP_OLDHELMET:
			m_fCurrentImage = 6;
			break;
		case EQUIP_OLDARMOR:
			m_fCurrentImage = 7;
			break;
		case EQUIP_OLDTROUSERS:
			m_fCurrentImage = 8;
			break;
		case EAT_POTION2:
			m_fCurrentImage = 24;
			break;
		case EAT_POTION7:
			m_fCurrentImage = 29;
			break;
		}
	}
	void		Set_FontTag(ITEMID _ItemID)
	{
		m_ItemID.eItemID = _ItemID;

		switch (m_ItemID.eItemID)
		{
		case WEAPON_SWORD:
			m_iCurrentFont = 1;
			m_iCurrentFontAlpha = 0;
			break;
		case WEAPON_BOW:
			m_iCurrentFont = 3;
			m_iCurrentFontAlpha = 2;
			break;
		case WEAPON_WAND3:
			m_iCurrentFont = 5;
			m_iCurrentFontAlpha = 4;
			break;
		case GENERAL_SHIELD:
			m_iCurrentFont = 7;
			m_iCurrentFontAlpha = 6;
			break;
		case GENERAL_LAMP:
			m_iCurrentFont = 9;
			m_iCurrentFontAlpha = 8;
			break;
		case EQUIP_OLDHELMET:
			m_iCurrentFont = 11;
			m_iCurrentFontAlpha = 10;
			break;
		case EQUIP_OLDARMOR:
			m_iCurrentFont = 13;
			m_iCurrentFontAlpha = 12;
			break;
		case EQUIP_OLDTROUSERS:
			m_iCurrentFont = 15;
			m_iCurrentFontAlpha = 14;
			break;
		case EAT_POTION2:
			m_iCurrentFont = 17;
			m_iCurrentFontAlpha = 16;
			break;
		case EAT_POTION7:
			m_iCurrentFont = 19;
			m_iCurrentFontAlpha = 18;
			break;
		}
	}
	virtual void		Key_Input();

private:
	HRESULT		Add_Component();

private:
	ITEMTYPEID	m_ItemID;
	CTexture* m_pTexture	 = nullptr;
	CTexture* m_pNameTexture = nullptr;
	_uint	  m_iRenderArray[8] = {};
	_uint	  m_iRenderCount = 0;

	_uint	  m_iCurrentFont;
	_uint	  m_iCurrentFontAlpha;
	_uint	  m_iCollisionNumber;

	_uint	  m_iBuyCount;
public:
	static CShopItem* Create(LPDIRECT3DDEVICE9 pGrahpicDev);

private:
	virtual void Free();
};

