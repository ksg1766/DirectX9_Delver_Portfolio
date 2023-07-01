#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeBf :	public CVIBuffer
{
public:
	explicit CCubeBf();
	explicit CCubeBf(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeBf(const CCubeBf& rhs);
	virtual ~CCubeBf();

public:
	virtual HRESULT			Ready_Buffer(void);
	virtual void			Render_Buffer(void);

public:
	static CCubeBf*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);

private:
	virtual	void	Free(void);
};

END