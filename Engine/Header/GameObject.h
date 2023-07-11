#pragma once

#include "Component.h"

BEGIN(Engine)

class CCollider;
class CBasicStat;
class CStateMachine;

class ENGINE_DLL CGameObject : public CBase
{
//protected:
public:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent*			Get_Component(COMPONENTTAG pComponentTag, COMPONENTID eID);
	OBJECTTAG			Get_ObjectTag()	 { return m_eObjectTag; }
	ITEMTAG				Get_ItemTag()	 { return m_eItemTag; }
	MONSTERTAG			Get_MonsterTag() { return m_eMonsterTag; }
	void				Set_ObjectTag(OBJECTTAG _eTag) { m_eObjectTag = _eTag; }

	_bool				IsDead() { return m_IsDead; }
	void				Set_Dead(_bool _IsDead) { m_IsDead = _IsDead; }

public:
	virtual HRESULT		Ready_Object(void);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void);
	virtual void		Render_Object(void)PURE;

public:
	CCollider*			Get_Collider() { return m_pCollider; }

	virtual void		OnCollisionEnter(CCollider* _pOther) {};
	virtual void		OnCollisionStay(CCollider* _pOther) {};
	virtual void		OnCollisionExit(CCollider* _pOther) {};

public:
	CBasicStat*			Get_BasicStat()		{ return m_pBasicStat; }
	STATE				Get_State()			{ return m_eState; }
	_bool				Get_AttackTick()	{ return m_bAttackTick; }
	CStateMachine*		Get_StateMachine()  { return m_pStateMachine; }

public:
	void				Set_State(STATE _eState) { m_eState = _eState; }
	void				Set_MonsterState(MONSTERTAG _eState) { m_eMonsterTag = _eState; }
	void				Set_AttackTick(_bool _Tick) { m_bAttackTick = _Tick; }

private:
	CComponent*			Find_Component(COMPONENTTAG pComponentTag, COMPONENTID eID);

public:
	CTransform*							m_pTransform = nullptr;	// Transform Attached to this GameObject

protected:
	multimap<COMPONENTTAG, CComponent*>	m_mapComponent[ID_END];
	CCollider*							m_pCollider = nullptr;
	CBasicStat*							m_pBasicStat = nullptr;
	CStateMachine*						m_pStateMachine = nullptr;

	LPDIRECT3DDEVICE9					m_pGraphicDev;
	_bool								m_IsDead;

	OBJECTTAG							m_eObjectTag;
	MONSTERTAG							m_eMonsterTag;
	ITEMTAG								m_eItemTag;
	STATE								m_eState;

	_bool								m_bAttackTick;

public:
	virtual void		Free();
};


END