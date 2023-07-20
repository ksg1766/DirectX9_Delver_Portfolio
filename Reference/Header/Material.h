#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMaterial :
    public CComponent
{
private:
	explicit CMaterial();
	explicit CMaterial(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMaterial(const CMaterial& rhs);
	virtual ~CMaterial();

public:

public:
	HRESULT			Ready_Material();
	virtual _int	Update_Component(const _float& fTimeDelta);

private:
	D3DMATERIAL9	m_tMaterial;

public:
	static CMaterial* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);

private:
	virtual void		Free();

};

END