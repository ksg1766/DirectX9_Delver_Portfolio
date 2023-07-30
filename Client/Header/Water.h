#pragma once
#include "CubeBlock.h"
class CWater :
    public CCubeBlock
{
protected:
	explicit CWater(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWater(const CWater& rhs);
	virtual ~CWater();

public:

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Object(void)						override;

	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

public:
	vector<_vec3>&		LoadCubeVertex() { return m_vecCubeVertex; }
	vector<INDEX32>&	LoadCubeIndex() { return m_vecCubeIndex; }

	_ubyte				Get_TextureNumber() { return m_iTextureNumber; }
	void				Set_TextureNumber(_uint _iTextureNumber) { m_iTextureNumber = _iTextureNumber; }

public:


private:
	HRESULT				Add_Component(void);

protected:

private:
	CCubeBf*	m_pBuffer = nullptr;
	_uint		m_iTextureNumber;

public:
	static CWater* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;
};

