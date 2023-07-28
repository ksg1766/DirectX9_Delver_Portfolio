#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;
END

class CBow : public Engine::CItem
{
private:
	explicit CBow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBow(const CBow& rhs);
	virtual ~CBow();

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
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture[(_uint)STATE::STATE_END] = {};
	CAnimator*	m_pAnimator = nullptr;

	_int		m_iAttackTick;
	_int		m_iMoveTick;
	_float		m_fChase2;
	_float		m_fChase;
	_float		m_fAngle;
	_int		m_iCount;

	// 공격 모션
	MYATTACKINFO m_AttackInfo;

public:
	static CBow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item);

private:
	virtual void Free() override;

};

