#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUILevelUpCard : public CTempUI
{
private:
	explicit CUILevelUpCard(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUILevelUpCard();

public:
	// 0¹ø - 5¹ø : AGILITY / ATTACK / DEFFENSE / HEALTH / MAGIC / SPEED
	void        Set_CardType(_uint _iType)     { m_iTypeImage = _iType; }
	void        Set_UseCard(_bool _bUse)       { m_bUseCard = _bUse; }
	void        Set_PickNumber(_uint _iNumber) { m_iPickNumber = _iNumber; }

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);
	void        Update_NumberUI();

private:
	CTexture*   m_pNumberTexture = nullptr;
	_uint       m_iTypeImage;
	_uint       m_iPickNumber;

	_uint       m_iCurrentStatNumber;
	_uint       m_iStatNumberOne;
	_uint       m_iStatNumberTwo;

	_bool       m_bUseCard = false;
	_bool       m_bStatUpdate = false;
	_bool       m_bMoveUpdate = false;
	_bool       m_bMouseCollision = false;

	_float      m_fFirstCardMoveTime = 0.f;

public:
	static CUILevelUpCard*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

