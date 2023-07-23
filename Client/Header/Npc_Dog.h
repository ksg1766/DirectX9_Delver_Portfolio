#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;
class CStateMachine;
class CFont;

END

class CNpc_Dog : public  Engine::CNpc
{
private:
	explicit CNpc_Dog(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_Dog(const CNpc_Dog& rhs);
	virtual ~CNpc_Dog();

public:
	virtual HRESULT	Ready_Object() override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object() override;
	virtual void	Render_Object() override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

public:
	_bool	IsTalk()	{ return m_bTalking; }

private:
	HRESULT	Add_Component();

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[(_uint)STATE::STATE_END] = {};
	CAnimator* m_pAnimator = nullptr;
	ID3DXFont* m_pFontconfig;
	CFont* m_pFont = nullptr;

	_bool	m_bTalkButton;
	_bool	m_bTalkBoX;
	_bool	m_bTalking;
	_bool	m_bStand;

public:
	static CNpc_Dog* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

