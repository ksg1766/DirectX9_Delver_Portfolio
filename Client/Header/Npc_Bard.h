#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;
class CStateMachine;
class CFont;

END

class CNpc_Bard : public  Engine::CNpc
{
private:
	explicit CNpc_Bard(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_Bard(const CNpc_Bard& rhs);
	virtual ~CNpc_Bard();

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

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[(_uint)STATE::STATE_END] = {};
	CAnimator* m_pAnimator = nullptr;
	CFont* m_pFont = nullptr;
	ID3DXFont* m_pFontconfig;

	_vec3			m_vPlayerPos;
	_vec3			m_vDir;
	_float			m_fDistance;

	_bool			m_bUseUI;
	_bool			m_bTalkButton;
	_bool			m_bTalkingBox;
	_float			m_fFontTime;
	CGameObject* m_pGameObject;
public:
	static CNpc_Bard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

