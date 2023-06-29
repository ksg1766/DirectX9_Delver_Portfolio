#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol :	public CVIBuffer
{
public:
	explicit CTriCol();
	explicit CTriCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTriCol(const CTriCol& rhs);
	virtual ~CTriCol();

public:
	virtual HRESULT			Ready_Buffer(void);
	virtual void			Render_Buffer(void);

public:
	static CTriCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);

private:
	virtual	void	Free(void);
};

END