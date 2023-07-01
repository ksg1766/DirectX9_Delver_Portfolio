#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CPlayerBf;
class CTransform;

END

class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);

private:
	CPlayerBf*		m_pBuffer = nullptr;
	CTransform*		m_pTransform = nullptr;

	//_vec3			m_vDir;
	_float			m_fSpeed = 10.f;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

