#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;
class CStateMachine;
class CFont;
class CTempUI;
END

class CNpc_OldMan : public  Engine::CNpc
{
private:
	explicit CNpc_OldMan(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_OldMan(const CNpc_OldMan& rhs);
	virtual ~CNpc_OldMan();

public:
	virtual HRESULT	Ready_Object() override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object() override;
	virtual void	Render_Object() override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);
	void			Set_UseUI(_bool _use) { m_bUseUI = _use; }
	_bool			Get_UseUI() { return m_bUseUI; }

	_bool			Get_Quest() { return m_bQuest; }
	_uint			Get_Speech() { return m_iSpeech; }

	_bool			IsTalk()	{ return m_bSpeech; }
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
	_bool			m_bQuest;//퀘스트를 줬는지 확인
	_bool			m_bSpeech;//rand가 자동없데이트 되는걸 막음.
	_bool			m_bFirstTalk = false;

	_float			m_fFontTime;
	_uint			m_iSpeech;
	CGameObject* m_pGameObject;
public:
	static CNpc_OldMan* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;
};

