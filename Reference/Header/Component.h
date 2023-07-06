#pragma once

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	void		Set_Transform(CTransform* _pTransform) { m_pTransform = _pTransform; }
	void		Set_ObjectTag(OBJECTTAG _eObject)	   { m_eObjectTag = _eObject; }

public:
	CTransform* Get_Transform()						  { return m_pTransform; }
	OBJECTTAG	Get_ObjectTag()						  { return m_eObjectTag; }

public:
	virtual	_int Update_Component(const _float& fTimeDelta) { return 0; }
	virtual void LateUpdate_Component(void) {}

public:
	void	Init_Property(CGameObject* _pHost);

protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bClone;

	CGameObject*			m_pHost;
	OBJECTTAG				m_eObjectTag;
	CTransform*				m_pTransform;	// Transform Attached to this GameObject

public:
	virtual CComponent*		Clone(void)		PURE;
	virtual void			Free(void)	override;
};

END