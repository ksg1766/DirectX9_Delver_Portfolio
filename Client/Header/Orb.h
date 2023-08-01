#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;
class CRigidBody;

END



class COrb : public Engine::CItem
{
private:
	explicit COrb(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit COrb(const COrb& rhs);
	virtual ~COrb();

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
	CRcTex*		m_pBuffer		= nullptr;
	CTexture*	m_pTexture		= nullptr;
	CRigidBody* m_pRigidBody	= nullptr;

	_int m_iMoveTick;



public:
	static COrb* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item);

private:
	virtual void Free() override;
};

