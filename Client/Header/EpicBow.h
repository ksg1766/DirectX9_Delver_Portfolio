#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CStateMachine;
class CAnimator;
class CRigidBody;

END

class CEpicBow : public Engine::CItem
{
private:
	explicit CEpicBow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEpicBow(const CEpicBow& rhs);
	virtual ~CEpicBow();

public:
	virtual HRESULT Ready_Object(_bool _Item);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT			Add_Component(void);

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[(_uint)STATE::STATE_END] = {};
	CAnimator* m_pAnimator = nullptr;
	CRigidBody* m_pRigidBody = nullptr;

	_int		m_iAttackTick;
	_float		m_fChase2;
	_float		m_fChase;
	_float		m_fAngle;
	_int		m_iCount;

	// 공격 모션
	MYATTACKINFO m_AttackInfo;

public:
	static CEpicBow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item);

private:
	virtual void Free() override;

};

