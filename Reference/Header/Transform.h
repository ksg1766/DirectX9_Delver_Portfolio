#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform :	public CComponent
{
private:
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	CTransform* Get_Parent() { return m_pParent; }
	void	Set_Parent(CTransform* _pParent) { m_pParent = _pParent; m_pParent->Set_Child(this); }

	void	Copy_RUL(_vec3* _Info) 
	{
		for (_uint i = 0; i < INFO_POS; ++i)
			CopyMemory(m_vInfo[i], _Info[i], sizeof(_vec3));
	}

	void	Copy_RUL_AddPos(_vec3* _Info)
	{
		for (_uint i = 0; i < INFO_END; ++i)
		{
			CopyMemory(m_vInfo[i], _Info[i], sizeof(_vec3));
		}
	}

	void	Scale(_vec3& _vEulers);
	void	Scale(const _vec3& _vEulers);

	void	Rotate(_vec3& _vEulers);
	void	Rotate(const _vec3& _vEulers);
	void	Rotate(const _float& _fXangle, const _float& _fYangle, const _float& _fZangle);
	void	Rotate(ROTATION eType, const _float& fAngle);
	void	RotateAround(const _vec3& _vPoint, const _vec3& _vAxis, const _float& _fAngle);	// Revolution;

	void	Translate(_vec3& _vTranslation);
	void	Translate(const _vec3& _vTranslation);

	const _matrix	WorldMatrix();
	void	Set_WorldMatrix(_matrix& _matWorld);

	_vec3	LocalScale() { return _vec3(m_vInfo[INFO_RIGHT].x, m_vInfo[INFO_UP].y, m_vInfo[INFO_LOOK].z); }
	vector<CTransform*>& Get_Child() { return m_pChild; }

public:
	HRESULT			Ready_Transform();
	virtual _int	Update_Component(const _float& fTimeDelta);

public:
	CTransform*		m_pParent;
	
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vLocalScale;

private:
	void					Set_Child(CTransform* _pChild) { m_pChild.push_back(_pChild); }
	vector<CTransform*>		m_pChild;

public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
private:
	virtual void			Free();

};

END