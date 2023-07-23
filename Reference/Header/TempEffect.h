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
	EFFECTTAG   Get_EffectTag() { return m_EffectTag; }
	void        Set_TargetObject(CGameObject* _pObject) { m_pTarget = _pObject; }
	void        Set_EffectColor(EFFECTCOLOR _Color) { m_EffectColor = _Color; }
	void        Set_RandomSet(_bool _RandomSetbool) { m_RandomSet = _RandomSetbool; }
	void        Set_CreateSet(_bool _CreateSetbool) { m_bCreateSet = _CreateSetbool; }
	void        Set_Parent(_bool _Parent)       { m_bParent = _Parent; }
	void        Set_Child(_bool _Child)         { m_bChild = _Child; }
	void        Set_EffectScale(_float _fScale) { m_fEffectScale = _fScale; }

public:
	HRESULT		Ready_Object();
	_int	    Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object(void);

protected:
	void Get_RandomVector(_vec3* out, _vec3* min, _vec3* max)
	{
		out->x = Get_RandomFloat(min->x, max->x);
		out->y = Get_RandomFloat(min->y, max->y);
		out->z = Get_RandomFloat(min->z, max->z);
	}
	float       Get_RandomFloat(float lowBound, float hightBound)
	{
		if (lowBound >= hightBound)
			return lowBound;

		float f = (rand() % 10000) * 0.0001f;

		return (f * (hightBound - lowBound)) + lowBound;
	}

protected:
	EFFECTTAG    m_EffectTag   = EFFECTTAG::EFFECT_END;
	EFFECTCOLOR  m_EffectColor = EFFECTCOLOR::ECOLOR_END;

	CGameObject* m_pTarget = nullptr;

	CRcTex*      m_pBufferCom;
	CTexture*    m_pTextureCom;
	CBillBoard*  m_pBillBoardCom;

	_float       m_fTime;

	_float       m_fFrame;
	_float       m_fFrist;
	_float       m_fFinal;
	_float       m_fFrameSpeed;

	_float       m_fLife;

	_bool        m_bAnimation;
	_bool        m_bLoop;
	_bool        m_bAniEnd;

	_bool        m_RandomSet;
	_bool        m_bScaleSet;
	_bool        m_bCreateSet;

	_bool        m_bParent;
	_bool        m_bChild;

	_float       m_fEffectScale;

protected:
	virtual void	Free(void);
};

END
