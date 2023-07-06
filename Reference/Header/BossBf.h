#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CBossBf :	public CVIBuffer
{
public:
	explicit CBossBf();
	explicit CBossBf(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBossBf(const CBossBf& rhs);
	virtual ~CBossBf();

public:
	virtual HRESULT			Ready_Buffer(void);
	virtual void			Render_Buffer(void);

public:
	static CBossBf*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);

private:
	virtual	void	Free(void);
};

END