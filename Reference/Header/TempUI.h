#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

class ENGINE_DLL CTempUI : public CGameObject
{
protected:
	explicit CTempUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTempUI(const CTempUI& rhs);
	virtual ~CTempUI();

public:
	const _matrix   WorldMatrix(float _fX, float _fY, float _fSizeX, float _fSizeY);
	void Set_Popup(_bool _Use)
	{
		m_IsDead = _Use;
	}

	HRESULT		Ready_Object();
	_int	    Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object(void);

protected:
	virtual HRESULT	Add_Component(void) { return S_OK; };
	virtual void	Key_Input(void) {};
	_bool           OnCollision(POINT& MousePoint, float _fX, float _fY, float _fSizeX, float _fSizeY);

protected:
	CRcTex*   m_pBufferCom;
	CTexture* m_pTextureCom;

	//_bool     m_bPopup;
	_bool     m_bCollider;

	_matrix   m_matWorld;
	_uint     m_fCurrentImage;
    //_float  m_fX;
    //_float  m_fY;
    //_float  m_fSizeX;
    //_float  m_fSizeY;

protected:
	virtual void	Free(void);
};

END
