#pragma once
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CFont;

END

class CUIUseShop_Trander : public CTempUI
{
private:
	explicit CUIUseShop_Trander(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIUseShop_Trander();

public:
	HRESULT	Ready_Object();
	_int	Update_Object(const _float& fTimeDelta);
	void	LateUpdate_Object();
	void	Render_Object();

private:
	HRESULT	Add_Component();
	void	Key_Input();

private:
	CFont*	   m_pFont = nullptr;
	ID3DXFont* m_pFontconfig;

public:
	static CUIUseShop_Trander* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();


};

