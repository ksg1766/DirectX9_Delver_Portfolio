#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicBf :
    public CVIBuffer
{
protected:
	explicit CDynamicBf();
	explicit CDynamicBf(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicBf(const CDynamicBf& rhs);
	virtual ~CDynamicBf();

public:
	virtual HRESULT	Ready_Buffer(void) override;
	virtual void	Render_Buffer(void) override;

public:
	static CDynamicBf* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
	virtual	void	Free(void);
};

END