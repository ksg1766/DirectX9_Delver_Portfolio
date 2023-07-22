#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CPlayerBf : public CVIBuffer
{
public:
	explicit CPlayerBf();
	explicit CPlayerBf(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerBf(const CPlayerBf& rhs);
	virtual ~CPlayerBf();

public:
	virtual HRESULT		Ready_Buffer(void);
	virtual void		Render_Buffer(void);

public:
	static CPlayerBf*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);

private:
	virtual	void	Free(void);
};

END