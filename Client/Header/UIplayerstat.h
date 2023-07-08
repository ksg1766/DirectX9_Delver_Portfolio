#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIplayerstat : public CTempUI
{
private:
	explicit CUIplayerstat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIplayerstat();

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);

private:
	CTexture* m_pNumberTextureCom;

	_matrix   m_matAttackPowerNumber;
	_matrix   m_matArmorClassNumber;

	_matrix   m_matHealthNumber;
	_matrix   m_matMagicNumber;
	_matrix   m_matSpeedNumber;

	_matrix   m_matAttackNumber;
	_matrix   m_matDefenseNumber;
	_matrix   m_matAgilityNumber;

	_matrix   m_matLevelNumber;
	_matrix   m_matXPNumber;
	_matrix   m_matGoldNumber;

	_uint     m_fAttackPowerNumber;
	_uint     m_fArmorClassNumber;

	_uint     m_fHealthNumber;
	_uint     m_fMagicNumber;
	_uint     m_fSpeedNumber;

	_uint     m_fAttackNumber;
	_uint     m_fDefenseNumber;
	_uint     m_fAgilityNumber;

	_uint     m_fLevelNumber;
	_uint     m_fXPNumber;
	_uint     m_fGoldNumber;

public:
	static CUIplayerstat*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

