#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;
class CFont;

END


class CPhantom : public Engine::CNpc
{
private:
	explicit CPhantom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPhantom(const CPhantom& rhs);
	virtual ~CPhantom();

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
	_bool		IsTalk() { return m_bTalking; }
	_bool		PhantomDead() { return m_bDead; }

	_bool		Get_QuestClear()	{ return m_bQuestClear; }
	_uint		Get_SpeechCount()	{ return m_iCount; }
private:
	HRESULT	Add_Component();
	void	Create_Puzzle();
private:
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture[(_uint)STATE::STATE_END] = {};
	CAnimator*	m_pAnimator = nullptr;
	CFont*		m_pFont = nullptr;
	ID3DXFont*	m_pFontconfig;

	_bool			m_bTalking;
	_bool			m_bTalkButton;
	_bool			m_bTalkingBox;
	
	_uint			m_iCount;
	_uint			m_iMaxCount;

	_vec3			m_vPushPos;
	_vec3			m_vInitPos;

	_bool			m_bPush = false;
	_bool			m_bQuestClear = false;
	_bool			m_bQusetStart = false;
	_bool			m_bFirstCollision = false;
	_bool			m_bDead = false;

public:
	static CPhantom* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

