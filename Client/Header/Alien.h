#pragma once

#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;

END

class CTerrain;

class CAlien : public CMonster
{
private:
	explicit CAlien(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAlien(const CAlien& rhs);
	virtual ~CAlien();

public:
	virtual HRESULT	Ready_Object()								override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object()							override;
	virtual void	Render_Object()								override;

public:
	void		ForceHeight(_vec3 _vPos);

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[(_uint)STATE::STATE_END] = {};
	CAnimator* m_pAnimator = nullptr;

	_float m_fFrame;

private:
	HRESULT		Add_Component(void);


public:
	static CAlien* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

