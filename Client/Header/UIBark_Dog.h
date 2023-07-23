#pragma once
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CFont;

END

class CUIBark_Dog : public CTempUI
{
private:
	explicit CUIBark_Dog(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIBark_Dog();

public:
	HRESULT	Ready_Object();
	_int	Update_Object(const _float& fTimeDelta);
	void	LateUpdate_Object();
	void	Render_Object();

private:
	HRESULT	Add_Component();
	void	Key_Input();

private:
	CFont* m_pFont = nullptr;
	ID3DXFont* m_pFontconfig;

public:
	static CUIBark_Dog* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();


};

