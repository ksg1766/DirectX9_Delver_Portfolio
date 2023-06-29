#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol :	public CVIBuffer
{
public:
	explicit CRcCol();
	explicit CRcCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcCol(const CRcCol& rhs);
	virtual ~CRcCol();

public:
	virtual HRESULT			Ready_Buffer(void);
	virtual void			Render_Buffer(void);

public:
	static CRcCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);

private:
	virtual	void	Free(void);
};

END