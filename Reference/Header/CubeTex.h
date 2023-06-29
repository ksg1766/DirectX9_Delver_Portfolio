#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex :	public CVIBuffer
{
public:
	explicit CCubeTex();
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex();

public:
	virtual HRESULT			Ready_Buffer(void);
	virtual void			Render_Buffer(void);

public:
	static CCubeTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);

private:
	virtual	void	Free(void);
};

END