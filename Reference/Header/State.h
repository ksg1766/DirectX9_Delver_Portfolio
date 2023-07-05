#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CState :
    public CComponent
{
private:
	explicit CState();
	explicit CState(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CState(const CState& rhs);
	virtual ~CState();

public:
	HRESULT			Ready_State();
	virtual void	LateUpdate_Component() override;

protected:

public:
	static	CState* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
private:
	virtual void			Free();
};

END