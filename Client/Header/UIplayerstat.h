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
	void        Update_NumverUI();
	void        Check_Number(_uint Number, _uint& NumberOne, _uint& NumberTwo);

private:
	// 복사 받은 해당 값
	_uint     m_fAttackPowerNumber;
	_uint     m_fArmorClassNumber;

	_uint     m_fHealthNumber;
	_uint     m_fMagicNumber;
	_uint     m_fSpeedNumber;

	_uint     m_fAttackNumber;
	_uint     m_fDefenseNumber;
	_uint     m_fAgilityNumber;

	_uint     m_fLevelNumber;
	_uint     m_fCurrentXPNumber;
	_uint     m_fMaxXPNumber;
	_uint     m_fGoldNumber;

	// 숫자 텍스처
	CTexture* m_pNumberTextureCom;

	// 숫자들의 출력 위치값
	_matrix   m_matAttackPowerNumberOne;
	_matrix   m_matAttackPowerNumberTwo;
	_matrix   m_matArmorClassNumberOne;
	_matrix   m_matArmorClassNumberTwo;

	_matrix   m_matHealthNumberOne;
	_matrix   m_matHealthNumberTwo;
	_matrix   m_matMagicNumberOne;
	_matrix   m_matMagicNumberTwo;
	_matrix   m_matSpeedNumberOne;
	_matrix   m_matSpeedNumberTwo;

	_matrix   m_matAttackNumberOne;
	_matrix   m_matAttackNumberTwo;
	_matrix   m_matDefenseNumberOne;
	_matrix   m_matDefenseNumberTwo;
	_matrix   m_matAgilityNumberOne;
	_matrix   m_matAgilityNumberTwo;

	_matrix   m_matLevelNumberOne;
	_matrix   m_matLevelNumberTwo;
	_matrix   m_matCurrentXPNumberOne;
	_matrix   m_matCurrentXPNumberTwo;
	_matrix   m_matSlash;
	_matrix   m_matMaxXPNumberOne;
	_matrix   m_matMaxXPNumberTwo;
	_matrix   m_matGoldNumberOne;
	_matrix   m_matGoldNumberTwo;

	// 값에 따른 출력하는 숫자
	_uint     m_fAttackPowerNumberOne;
	_uint     m_fAttackPowerNumberTwo;
	_uint     m_fArmorClassNumberOne;
	_uint     m_fArmorClassNumberTwo;

	_uint     m_fHealthNumberOne;
	_uint     m_fHealthNumberTwo;
	_uint     m_fMagicNumberOne;
	_uint     m_fMagicNumberTwo;
	_uint     m_fSpeedNumberOne;
	_uint     m_fSpeedNumberTwo;

	_uint     m_fAttackNumberOne;
	_uint     m_fAttackNumberTwo;
	_uint     m_fDefenseNumberOne;
	_uint     m_fDefenseNumberTwo;
	_uint     m_fAgilityNumberOne;
	_uint     m_fAgilityNumberTwo;

	_uint     m_fLevelNumberOne;
	_uint     m_fLevelNumberTwo;
	_uint     m_fCurrentXPNumberOne;
	_uint     m_fCurrentXPNumberTwo;
	_uint     m_fMaxXPNumberOne;
	_uint     m_fMaxXPNumberTwo;
	_uint     m_fGoldNumberOne;
	_uint     m_fGoldNumberTwo;

public:
	static CUIplayerstat*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

