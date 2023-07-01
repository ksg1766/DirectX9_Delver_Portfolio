#include "..\..\Header\TerrainTex.h"

CTerrainTex::CTerrainTex()
{
}

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:  CVIBuffer(pGraphicDev)
{
}

CTerrainTex::CTerrainTex(const CTerrainTex & rhs)
	: CVIBuffer(rhs)
	, m_hFile(rhs.m_hFile)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
{
}


CTerrainTex::~CTerrainTex()
{
}

HRESULT CTerrainTex::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	///////////////////////////////////
	/*LPDIRECT3DTEXTURE9		pTexture = nullptr;

	D3DXCreateTexture(m_pGraphicDev, 129, 129, 1, 
		0, // D3DUSAGE_RENDERTARGET 을 지정할 경우 렌더 타겟 용 텍스처를 생성
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture);

	D3DLOCKED_RECT		LockRect;

	pTexture->LockRect(0, &LockRect, nullptr, 0);

	for (int i = 0; i < 129; ++i)
	{
		for (int j = 0; j < 129; ++j)
		{
			_int		iIndex = i * 129 + j;
			*(((_ulong*)LockRect.pBits) + iIndex) = D3DCOLOR_ARGB(255, 0, 255, 0);
		}
	}
	
	pTexture->UnlockRect(0);

	D3DXSaveTextureToFile(L"../Bin/Resource/Texture/Terrain/137bmp.bmp", D3DXIFF_BMP, pTexture, NULL);*/

	m_dwFVF = VTXTEX::format;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	m_hFile = CreateFile(L"../Bin/Resource/Texture/Terrain/Height2.bmp", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	_ulong	dwByte = 0;

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	_ulong*		pPixel = new _ulong[m_iH.biHeight * m_iH.biWidth];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biHeight * m_iH.biWidth, &dwByte, NULL);

	CloseHandle(m_hFile);

	VTXTEX*		pVertex = nullptr;

	_ulong		dwIndex = 0;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPosition = { _float(j * dwVtxItv), 
				0.f/*_float(pPixel[dwIndex] & 0x000000ff) / 30.f*/,
				_float(i * dwVtxItv) };
			pVertex[dwIndex].vTexUV = {_float(j) / (dwCntX - 1)  * 20.f, 
										_float(i) / (dwCntZ - 1) * 20.f };			
		}
	}	
	m_pVB->Unlock();

	Safe_Delete_Array(pPixel);

	_ulong		dwTriCnt = 0;

	INDEX32*			pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;
			dwTriCnt++;

			// 왼쪽 아래
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;
			dwTriCnt++;
		}
	}	

	m_pIB->Unlock();




	return S_OK;
}

void CTerrainTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CTerrainTex * CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const _ulong& dwCntX ,
									const _ulong& dwCntZ ,
									const _ulong& dwVtxItv )
{
	CTerrainTex *	pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pInstance);
		MSG_BOX("TerrainTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

void CTerrainTex::Free(void)
{
	CVIBuffer::Free();
}
