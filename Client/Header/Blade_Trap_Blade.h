#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CStateMachine;
class CAnimator;
END

class CBlade_Trap_Blade :
    public CGameObject
{
private:
	explicit	CBlade_Trap_Blade(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBlade_Trap_Blade(const CBlade_Trap_Blade& rhs);
	virtual		~CBlade_Trap_Blade();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

	void		Set_Collider();
	void		Set_TrapCenter(_vec3 _vCenter) { m_vTrapCenter = _vCenter; }
	_vec3		Get_TrapCenter() { return m_vTrapCenter; }
public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);
private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[(_uint)STATE::STATE_END] = {};
	CStateMachine* m_pStateMachine = nullptr;
	CAnimator* m_pAnimator = nullptr;
	CGameObject*	m_pOtherObj = nullptr;
	CGameObject*	m_pHostObject = nullptr;
	_vec3			m_vPlayerPos;
	_vec3			m_vDir;
	_vec3			m_vTrapCenter;
	_float			m_fDistance;
	_float			m_fHitTime;
	_bool			m_bHit;
	_bool			m_bCollider;
private:
	HRESULT		Add_Component(void);
public:
	static CBlade_Trap_Blade* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

