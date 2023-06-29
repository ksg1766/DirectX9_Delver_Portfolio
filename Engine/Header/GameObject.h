#pragma once

#include "Component.h"

BEGIN(Engine)

class CCollider;
class ENGINE_DLL CGameObject : public CBase
{
//protected:
public:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent*			Get_Component(COMPONENTTAG pComponentTag, COMPONENTID eID);
	OBJECTTAG			Get_ObjectTag() { return m_eObjectTag; }

	_bool				IsDead() { return m_IsDead; }
	void				Set_Dead(_bool _IsDead) { m_IsDead = _IsDead; }

public:
	virtual HRESULT		Ready_Object(void);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void);
	virtual void		Render_Object(void)PURE;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther) {};
	virtual void		OnCollisionStay(CCollider* _pOther) {};
	virtual void		OnCollisionExit(CCollider* _pOther) {};

private:
	CComponent*			Find_Component(COMPONENTTAG pComponentTag, COMPONENTID eID);

protected:
	map<COMPONENTTAG, CComponent*>		m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	_bool								m_IsDead;
	OBJECTTAG							m_eObjectTag;

public:
	virtual void		Free();
};


END