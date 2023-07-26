#pragma once

#include "Monster.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;
class CBossAI;
class CAnimator;
END

class CSkeletonKing_Clone : public Engine::CMonster
{
private:
	explicit	CSkeletonKing_Clone(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CSkeletonKing_Clone(const CSkeletonKing_Clone& rhs);
	virtual		~CSkeletonKing_Clone();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);
private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[(_uint)STATE::STATE_END] = {};
	CAnimator*	m_pAnimator;
	_float	m_fFrame = 0;
	_float	m_fSkillCool = 0;
	_float	m_fDelay;
	_float	m_fDeleteDelay;
	_bool	m_bHit;

private:
	HRESULT		Add_Component(void);

public:
	static CSkeletonKing_Clone* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

	// CMonster을(를) 통해 상속됨
	virtual void Init_Stat() override;
};

