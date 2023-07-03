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
	void	Set_Parent(CTransform* _pParent) { m_pParent = _pParent; }

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

	/*
	void	Move_Pos(const _vec3* const pDir, const _float& fTimeDelta, const _float& fSpeed)
			{ m_vInfo[INFO_POS] += *pDir * fTimeDelta * fSpeed; }

	void	Chase_Target(const _vec3* pTargetPos, const _float& fTimeDelta, const _float& fSpeed);
	const _matrix*		Compute_LookAtTarget(const _vec3* pTargetPos);
	*/

public:
	HRESULT			Ready_Transform();
	virtual _int	Update_Component(const _float& fTimeDelta);

public:
	CTransform*		m_pParent;

	_vec3			m_vInfo[INFO_END];
	_vec3			m_vAngle;
	_vec3			m_vLocalScale;

	//_matrix			m_matWorld;

public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
private:
	virtual void			Free();

};

END