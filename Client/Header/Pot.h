#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;
class CStateMachine;
class CFont;

END
//소비 아이템 나오게 할 예정
class CPot :
    public CGameObject
{
private:
	explicit CPot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPot(const CPot& rhs);
	virtual ~CPot();
public:
	virtual HRESULT	Ready_Object() override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object() override;
	virtual void	Render_Object() override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	HRESULT	Add_Component();
	void	Drop_RandomItem();
private:
	_bool		m_bCrash;
	_bool		m_bHit;
	_float		m_fHitCool;
	CGameObject* m_pOtherObj;
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;

public:
	static CPot* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;
};

