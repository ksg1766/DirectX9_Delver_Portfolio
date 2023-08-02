#pragma once
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CFont;

END

class CUISpeech_Wizard : public CTempUI
{
private:
	explicit CUISpeech_Wizard(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUISpeech_Wizard();

public:
	HRESULT	Ready_Object();
	_int	Update_Object(const _float& fTimeDelta);
	void	LateUpdate_Object();
	void	Render_Object();

private:
	HRESULT	Add_Component();
	void	Key_Input();

private:
	CFont* m_pFont[3];
	ID3DXFont* m_pFontconfig[3];

public:
	static CUISpeech_Wizard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();


};

