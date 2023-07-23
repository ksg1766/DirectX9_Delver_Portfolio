#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIitem : public CTempUI
{
private:
	explicit CUIitem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIitem();

public:
	ITEMTYPEID  Get_ItemTag() { return m_ItemID; }
	void        Add_ItemCount(_uint _iCount) {
		m_ItemID.iCount += _iCount;
	}
	void        Remove_ItemCount(_uint _iCount) {
		m_ItemID.iCount -= _iCount;
	}
	void        Set_ItemTag(ITEMTYPE _itemtype, ITEMID _itemID, _uint _iCount) {
		m_ItemID.eItemType = _itemtype;
		m_ItemID.eItemID = _itemID;
		m_ItemID.iCount = _iCount;

		switch (m_ItemID.eItemID)
		{
		case WEAPON_SWORD:
			m_fCurrentImage = 30;
			break;
		case WEAPON_BOW:
			m_fCurrentImage = 33;
			//m_UINumber = 0; // 테스트용
			break;
		case WEAPON_ARROW:
			m_fCurrentImage = 34;
			break;
		case WEAPON_BOWANDARROW:
			m_fCurrentImage = 35;
			break;
		case WEAPON_WAND1:
			m_fCurrentImage = 36;
			break;
		case WEAPON_WAND2:
			m_fCurrentImage = 37;
			break;
		case WEAPON_WAND3:
			m_fCurrentImage = 38;
			break;
		case WEAPON_WAND4:
			m_fCurrentImage = 39;
			break;
		case WEAPON_WAND5:
			m_fCurrentImage = 40;
			break;
		case GENERAL_SHIELD:
			m_fCurrentImage = 31;
			m_UINumber = 1;
			break;
		case GENERAL_LAMP:
			m_fCurrentImage = 17;
			m_UINumber = 1;
			break;
		case GENERAL_BEER:
			m_fCurrentImage = 18;
			m_UINumber = 1;
			break;
		case GENERAL_KEY:
			m_fCurrentImage = 19;
			break;
		case GENERAL_DOCUMENT:
			m_fCurrentImage = 20;
			break;
		case GENERAL_MAP:
			m_fCurrentImage = 22;
			break;
		case EQUIP_OLDHELMET:
			m_fCurrentImage = 6;
			m_UINumber = 0;
			break;
		case EQUIP_OLDARMOR:
			m_fCurrentImage = 7;
			m_UINumber = 2;
			break;
		case EQUIP_OLDTROUSERS:
			m_fCurrentImage = 8;
			m_UINumber = 4;
			break;
		case EQUIP_IRONHELMET:
			m_fCurrentImage = 9;
			m_UINumber = 0;
			break;
		case EQUIP_IRONARMOR:
			m_fCurrentImage = 10;
			m_UINumber = 2;
			break;
		case EQUIP_IRONTROUSERS:
			m_fCurrentImage = 11;
			m_UINumber = 4;
			break;
		case EQUIP_SMALLSILVERRING:
			m_fCurrentImage = 0;
			m_UINumber = 3;
			break;
		case EQUIP_BIGSILVERRING:
			m_fCurrentImage = 4;
			m_UINumber = 3;
			break;
		case EQUIP_SILVERNECKLACE:
			m_fCurrentImage = 1;
			m_UINumber = 5;
			break;
		case EQUIP_SMALLGOLDRING:
			m_fCurrentImage = 2;
			m_UINumber = 3;
			break;
		case EQUIP_BIGGOLDRING:
			m_fCurrentImage = 5;
			m_UINumber = 3;
			break;
		case EQUIP_GOLDNECKLACE:
			m_fCurrentImage = 3;
			m_UINumber = 5;
			break;
		case EAT_SLICEDBREAD:
			m_fCurrentImage = 12;
			break;
		case EAT_APPLE:
			m_fCurrentImage = 13;
			break;
		case EAT_CHEESE:
			m_fCurrentImage = 14;
			break;
		case EAT_RAWMEAT:
			m_fCurrentImage = 15;
			break;
		case EAT_COOKEDMEAT:
			m_fCurrentImage = 16;
			break;
		case EAT_POTION1:
			m_fCurrentImage = 23;
			break;
		case EAT_POTION2:
			m_fCurrentImage = 24;
			break;
		case EAT_POTION3:
			m_fCurrentImage = 25;
			break;
		case EAT_POTION4:
			m_fCurrentImage = 26;
			break;
		case EAT_POTION5:
			m_fCurrentImage = 27;
			break;
		case EAT_POTION6:
			m_fCurrentImage = 28;
			break;
		case EAT_POTION7:
			m_fCurrentImage = 29;
			break;
		case WEAPON_EPICBOW:
			m_fCurrentImage = 41;
			break;
		}
	}

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);
	void        Update_NumverUI();

private:
	ITEMTYPEID  m_ItemID;

	CTexture*   m_pNumberTextureCom;
	CTexture*   m_pTooltipCom;

	_int        m_iCurrentOneNum;
	_int        m_iCurrentTwoNum;

	_float      m_fTooltipPosX;
	_float      m_fTooltipPosY;
	_bool       m_bTooltipRender = false;

	_bool       m_bMove = false;

	_matrix     m_Position;

public:
	static CUIitem*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();
};

