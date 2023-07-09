#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
public:
	explicit CTexture();
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	HRESULT			Ready_Texture(TEXTUREID eType, const _tchar* pPath, const _uint& iCnt);
	void			Render_Texture(const _uint& iIndex = 0);

public:
	vector<IDirect3DBaseTexture9*>&	Get_TextureList() { return m_vecTexture; }

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	_int	iSize = 0;

public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType, _tchar* pPath, const _uint& iCnt = 1);
	virtual CComponent*		Clone();

private:
	virtual void	Free();
};

END