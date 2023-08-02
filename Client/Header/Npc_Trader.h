#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;
class CStateMachine;
class CFont;

END

class CNpc_Trader : public  Engine::CNpc
{
private:
	explicit CNpc_Trader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_Trader(const CNpc_Trader& rhs);
	virtual ~CNpc_Trader();

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
	_bool	IsTalk()		{ return m_bTalking; }

private:
	HRESULT	Add_Component();

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[(_uint)STATE::STATE_END] = {};
	CAnimator* m_pAnimator = nullptr;
	CFont* m_pFont = nullptr;
	ID3DXFont* m_pFontconfig;

	_bool	m_bTalkButton;
	_bool	m_bTalkBox;
	_bool	m_bTalking;
	_bool	m_bUse;
	_bool	m_bFirstTalk = false;
public:
	static CNpc_Trader* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

