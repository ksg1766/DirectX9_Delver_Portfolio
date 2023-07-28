#pragma once
#include "CubeBlock.h"
class CWaterFall :
    public CCubeBlock
{
protected:
	explicit CWaterFall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWaterFall(const CWaterFall& rhs);
	virtual ~CWaterFall();

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

	_ubyte				Get_TextureNumber() { return m_byTextureNumber; }
	void				Set_TextureNumber(_uint _iTextureNumber) { m_byTextureNumber = _iTextureNumber; }

private:
	HRESULT				Add_Component(void);

protected:

private:
	CCubeBf* m_pBuffer = nullptr;

public:
	static CWaterFall* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free() override;
};

