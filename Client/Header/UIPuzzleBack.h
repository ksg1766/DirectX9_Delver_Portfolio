#pragma once

#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIPuzzle;

class CUIPuzzleBack : public CTempUI
{
private:
	explicit CUIPuzzleBack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIPuzzleBack();

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

public:
	vector<CUIPuzzle*>& Get_UIPuzzle()	{ return m_vecPuzzle; }

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);

private:
	vector<CUIPuzzle*> m_vecPuzzle;


public:
	static CUIPuzzleBack* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

