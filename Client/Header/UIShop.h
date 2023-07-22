#pragma once
#include "TempUI.h"
#include "ShopItem.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIShop : public CTempUI
{
private:
	explicit CUIShop(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIShop();
	
public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object();
	void		Render_Object();


private:
	HRESULT		Add_Component();

private:
	ITEMTYPEID m_ItemID;
	vector<CTempUI*> vecShopItem;

public:
	static CUIShop* Create(LPDIRECT3DDEVICE9 pGrahpicDev);

private:
	virtual void Free();
};

