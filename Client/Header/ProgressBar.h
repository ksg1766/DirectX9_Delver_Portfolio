#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CProgressBar : public CTempUI
{
private:
	explicit CProgressBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CProgressBar();

public:
	_bool Get_Finish() { return m_bFinish; }

public:
	HRESULT Ready_Object() override;
	_int Update_Object(const _float& fTimeDelta) override;
	void LateUpdate_Object(void) override;
	void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);
	void        Update_BarUI();

private:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;

	_float          m_fCurrentBar;
	_float          m_fMaxBar;
	_float          m_fPosition;
	_float          m_flength;

	_bool           m_bFinish = false;

public:
	static CProgressBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

