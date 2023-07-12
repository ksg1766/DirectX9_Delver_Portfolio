#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUItooltip : public CTempUI
{
private:
	explicit CUItooltip(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUItooltip();

public:
	ITEMID      Get_itemID() { return m_ItemID.eItemID; }
	void        Set_ItemTag(ITEMID _itemID)
	{
		switch (_itemID)
		{
		case Engine::WEAPON_SWORD:
			m_ItemID.eItemID = WEAPON_SWORD;
			m_ImageNum = 30;
			break;
		case Engine::WEAPON_BOW:
			m_ItemID.eItemID = WEAPON_BOW;
			m_ImageNum = 33;
			break;
		case Engine::WEAPON_ARROW:
			m_ItemID.eItemID = WEAPON_ARROW;
			m_ImageNum = 34;
			break;
		case Engine::WEAPON_BOWANDARROW:
			m_ItemID.eItemID = WEAPON_BOWANDARROW;
			m_ImageNum = 35;
			break;
		case Engine::WEAPON_WAND1:
			m_ItemID.eItemID = WEAPON_WAND1;
			m_ImageNum = 36;
			break;
		case Engine::WEAPON_WAND2:
			m_ItemID.eItemID = WEAPON_WAND2;
			m_ImageNum = 37;
			break;
		case Engine::WEAPON_WAND3:
			m_ItemID.eItemID = WEAPON_WAND3;
			m_ImageNum = 38;
			break;
		case Engine::WEAPON_WAND4:
			m_ItemID.eItemID = WEAPON_WAND4;
			m_ImageNum = 39;
			break;
		case Engine::WEAPON_WAND5:
			m_ItemID.eItemID = WEAPON_WAND5;
			m_ImageNum = 40;
			break;
		case Engine::GENERAL_LAMP:
			m_ItemID.eItemID = GENERAL_LAMP;
			m_ImageNum = 17;
			break;
		case Engine::GENERAL_BEER:
			m_ItemID.eItemID = GENERAL_BEER;
			m_ImageNum = 18;
			break;
		case Engine::GENERAL_KEY:
			m_ItemID.eItemID = GENERAL_KEY;
			m_ImageNum = 19;
			break;
		case Engine::GENERAL_DOCUMENT:
			m_ItemID.eItemID = GENERAL_DOCUMENT;
			m_ImageNum = 20;
			break;
		case Engine::GENERAL_MAP:
			m_ItemID.eItemID = GENERAL_MAP;
			m_ImageNum = 22;
			break;
		case Engine::EQUIP_OLDHELMET:
			m_ItemID.eItemID = EQUIP_OLDHELMET;
			m_ImageNum = 6;
			break;
		case Engine::EQUIP_OLDARMOR:
			m_ItemID.eItemID = EQUIP_OLDARMOR;
			m_ImageNum = 7;
			break;
		case Engine::EQUIP_OLDTROUSERS:
			m_ItemID.eItemID = EQUIP_OLDTROUSERS;
			m_ImageNum = 8;
			break;
		case Engine::EQUIP_IRONHELMET:
			m_ItemID.eItemID = EQUIP_IRONHELMET;
			m_ImageNum = 9;
			break;
		case Engine::EQUIP_IRONARMOR:
			m_ItemID.eItemID = EQUIP_IRONARMOR;
			m_ImageNum = 10;
			break;
		case Engine::EQUIP_IRONTROUSERS:
			m_ItemID.eItemID = EQUIP_IRONTROUSERS;
			m_ImageNum = 11;
			break;
		case Engine::EQUIP_SMALLSILVERRING:
			m_ItemID.eItemID = EQUIP_SMALLSILVERRING;
			m_ImageNum = 0;
			break;
		case Engine::EQUIP_BIGSILVERRING:
			m_ItemID.eItemID = EQUIP_BIGSILVERRING;
			m_ImageNum = 4;
			break;
		case Engine::EQUIP_SILVERNECKLACE:
			m_ItemID.eItemID = EQUIP_SILVERNECKLACE;
			m_ImageNum = 1;
			break;
		case Engine::EQUIP_SMALLGOLDRING:
			m_ItemID.eItemID = EQUIP_SMALLGOLDRING;
			m_ImageNum = 2;
			break;
		case Engine::EQUIP_BIGGOLDRING:
			m_ItemID.eItemID = EQUIP_BIGGOLDRING;
			m_ImageNum = 5;
			break;
		case Engine::EQUIP_GOLDNECKLACE:
			m_ItemID.eItemID = EQUIP_GOLDNECKLACE;
			m_ImageNum = 3;
			break;
		case Engine::EAT_SLICEDBREAD:
			m_ItemID.eItemID = EAT_SLICEDBREAD;
			m_ImageNum = 12;
			break;
		case Engine::EAT_APPLE:
			m_ItemID.eItemID = EAT_APPLE;
			m_ImageNum = 13;
			break;
		case Engine::EAT_CHEESE:
			m_ItemID.eItemID = EAT_CHEESE;
			m_ImageNum = 14;
			break;
		case Engine::EAT_RAWMEAT:
			m_ItemID.eItemID = EAT_RAWMEAT;
			m_ImageNum = 15;
			break;
		case Engine::EAT_COOKEDMEAT:
			m_ItemID.eItemID = EAT_COOKEDMEAT;
			m_ImageNum = 16;
			break;
		case Engine::EAT_POTION1:
			m_ItemID.eItemID = EAT_POTION1;
			m_ImageNum = 23;
			break;
		case Engine::EAT_POTION2:
			m_ItemID.eItemID = EAT_POTION2;
			m_ImageNum = 24;
			break;
		case Engine::EAT_POTION3:
			m_ItemID.eItemID = EAT_POTION3;
			m_ImageNum = 25;
			break;
		case Engine::EAT_POTION4:
			m_ItemID.eItemID = EAT_POTION4;
			m_ImageNum = 26;
			break;
		case Engine::EAT_POTION5:
			m_ItemID.eItemID = EAT_POTION5;
			m_ImageNum = 27;
			break;
		case Engine::EAT_POTION6:
			m_ItemID.eItemID = EAT_POTION6;
			m_ImageNum = 28;
			break;
		case Engine::EAT_POTION7:
			m_ItemID.eItemID = EAT_POTION7;
			m_ImageNum = 29;
			break;
		}
	}
	void        Set_TooltipPos(POINT _pos)
	{
		// 좌측 하단에 위치하도록 만들고 가지고 있는 행렬 값 변경


	}

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);

private:
	ITEMTYPEID  m_ItemID;
	_uint       m_ImageNum;

public:
	static CUItooltip*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

