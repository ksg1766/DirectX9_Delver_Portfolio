#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeColBf :
    public CVIBuffer
{
public:
	explicit CCubeColBf();
	explicit CCubeColBf(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeColBf(const CCubeColBf& rhs);
	virtual ~CCubeColBf();

public:
	virtual HRESULT			Ready_Buffer(void);
	virtual void			Render_Buffer(void);

public:
	static CCubeColBf* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);

private:
	virtual	void	Free(void);
};

END