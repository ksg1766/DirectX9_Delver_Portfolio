#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBasicStat;
class CBillBoard;

END

class CShield : public Engine::CItem
{
private:
	explicit CShield(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShield(const CShield& rhs);
	virtual ~CShield();

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
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;


	_int		m_iAttackTick;
	_int		m_iMoveTick;
	_vec3		m_vDir;

	_vec3		m_vBoomerangPos;
	_float		m_fAngle;
	_float		m_fTime;
	_float		m_fAngleVelocity;
public:
	static CShield* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item);

private:
	virtual void Free() override;
};

