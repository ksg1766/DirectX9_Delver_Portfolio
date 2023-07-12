#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCubeBf;
class CTexture;
class CCollider;

END

class CBox_Cube : public Engine::CGameObject
{
private:
	explicit CBox_Cube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBox_Cube(const CBox_Cube& rhs);
	virtual ~CBox_Cube();

public:

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

public:

private:
	HRESULT		Add_Component(void);
	void		Shake_Box(const _float& fTimeDelta);
private:
	CCubeBf*	m_pBuffer = nullptr;
	CTexture*	m_pTexture = nullptr;
	_ubyte		m_iTextureNumber;
	_float		m_fHitCool;
	_float		m_fShakeCool;
	_bool		m_bHit;
	_bool		m_bShake;
public:
	static CBox_Cube* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};
