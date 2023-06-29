#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex :	public CVIBuffer
{
public:
	explicit CTerrainTex();
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();

public:
	HRESULT			Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	void			Render_Buffer(void);

private:
	HANDLE			m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

public:
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const _ulong& dwCntX = VTXCNTX, 
									const _ulong& dwCntZ = VTXCNTZ, 
									const _ulong& dwVtxItv = VTXITV);

	virtual CComponent*	Clone(void);

private:
	virtual	void	Free(void);
};

END