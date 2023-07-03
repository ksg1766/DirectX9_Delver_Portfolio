#pragma once
#include "Component.h"

BEGIN(Engine)

enum class LIGHT_TYPE {LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_SPOT, LIGHT_END};

class ENGINE_DLL CLight :
    public CComponent
{
private:
	explicit CLight();
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLight(const CLight& rhs);
	virtual ~CLight();

public:
	void			Light_Switch() { m_bOn = !m_bOn; }
	

public:
	HRESULT			Ready_Light(LIGHT_TYPE _eLightType);
	virtual _int	Update_Component(const _float& fTimeDelta);

private:
	D3DLIGHT9		m_tLight;
	_bool			m_bOn;


public:
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
private:
	virtual void			Free();

};

END