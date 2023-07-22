#pragma once
#include "CubeBLock.h"

BEGIN(Engine)

class CDynamicBf;
class CTransform;
class CTexture;
class CCollider;

END

class CDynamicCubeBlock :
    public CCubeBlock
{
private:
	explicit CDynamicCubeBlock(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicCubeBlock(const CDynamicCubeBlock& rhs);
	virtual ~CDynamicCubeBlock();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT				Add_Component(void);
	void				WaterFlow();

//protected:
//	CCubeBf*			m_pBuffer = nullptr;
//	CTexture*			m_pTexture = nullptr;
//	_ubyte				m_byTextureNumber;
//
//	vector<_vec3>		m_vecCubeVertex;
//	vector<INDEX32>		m_vecCubeIndex;

private:
	CDynamicBf*			m_pBuffer = nullptr;

public:
	static CDynamicCubeBlock* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

