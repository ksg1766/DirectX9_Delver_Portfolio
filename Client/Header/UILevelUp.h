#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUILevelUp : public CTempUI
{
private:
	explicit CUILevelUp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUILevelUp();

public:
	void        Set_UseLevelUpUI(_bool _bUse)       { m_bUseLevelUpUI = _bUse; }
	void        Set_NotUseLevelUpUI(_bool _bNotUse) { m_bNotUseLevelUpUI = _bNotUse; }

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);
	float       Get_RandomFloatNumber(float lowBound, float hightBound) {
		if (lowBound >= hightBound)
			return lowBound;

		float f = (rand() % 10000) * 0.0001f;

		return (f * (hightBound - lowBound)) + lowBound;
	}

private:
	_matrix     m_FontWorld;
	_bool       m_bUseLevelUpUI    = false;
	_bool       m_bNotUseLevelUpUI = false;

	vector<CGameObject*> m_vecCardObject;
	vector<CGameObject*> m_vecPickObject;
	_bool                m_bSelectCard[6];
	_bool                m_bPickRender = false;


public:
	static CUILevelUp*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

