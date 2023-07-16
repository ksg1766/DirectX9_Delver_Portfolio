#pragma once
#include "Export_Function.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

class ENGINE_DLL CTempEffect : public CGameObject
{
protected:
	explicit CTempEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTempEffect(const CTempEffect& rhs);
	virtual ~CTempEffect();

public:
	void        Set_RandomSet(_bool _RandomSetbool) { m_RandomSet = _RandomSetbool; }

public:
	HRESULT		Ready_Object();
	_int	    Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object(void);

protected:
	float Get_RandomFloat(float lowBound, float hightBound)
	{
		if (lowBound >= hightBound)
			return lowBound;

		float f = (rand() % 10000) * 0.0001f;

		return (f * (hightBound - lowBound)) + lowBound;
	}

protected:
	CRcTex*      m_pBufferCom;
	CTexture*    m_pTextureCom;
	CBillBoard*  m_pBillBoardCom;

	_float       m_fTime;

	_float       m_fFrame;
	_float       m_fFrist;
	_float       m_fFinal;
	_float       m_fFrameSpeed;

	_float       m_fLife;

	_bool        m_bLoop;
	_bool        m_RandomSet;

	_float       m_fEffectScale;

protected:
	virtual void	Free(void);
};

END
