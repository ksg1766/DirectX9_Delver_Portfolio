#pragma once
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CFont;

END

class CUISpeech_Alchemist : public CTempUI
{
private:
	explicit CUISpeech_Alchemist(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUISpeech_Alchemist();

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
	static CUISpeech_Alchemist* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();


};

