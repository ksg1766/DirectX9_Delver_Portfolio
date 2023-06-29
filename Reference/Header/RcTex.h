#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex :	public CVIBuffer
{
public:
	explicit CRcTex();
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex();

public:
	virtual HRESULT			Ready_Buffer(void);
	virtual void			Render_Buffer(void);

public:
	static CRcTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);

private:
	virtual	void	Free(void);
};

END