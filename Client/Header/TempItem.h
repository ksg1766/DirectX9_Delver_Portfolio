#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBasicStat;

END

class CTempItem :
    public Engine::CItem
{
private:
	explicit CTempItem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTempItem(const CTempItem& rhs);
	virtual ~CTempItem();

public:
	virtual HRESULT Ready_Object(void) override;
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
	CTexture*	m_pTexture = nullptr;
	_float		m_fSignTime;

	// 공격 모션
	MYATTACKINFO m_AttackInfo;

public:
	static CTempItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

