#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCubeBf;
class CTexture;
class CCollider;
class CRigidBody;

END
//부시면 장비아이템 나오게 할 예정
class CEquipBox : public Engine::CGameObject
{
private:
	explicit CEquipBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEquipBox(const CEquipBox& rhs);
	virtual ~CEquipBox();

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
	void		Drop_RandomItem();
	void		CreateConsum();
private:
	CCubeBf* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CGameObject* m_pOtherObj;
	CRigidBody* m_pRigidBody = nullptr;
	_ubyte		m_iTextureNumber;
	_float		m_fHitCool;
	_float		m_fShakeDelay;
	_bool		m_bHit;
	_bool		m_bShake;
	_int		m_iHP;

	_vec3		m_vOriginUp;
	_vec3		m_vOriginLook;
	_vec3		m_vOriginRight;
	_float		m_fLerpTime;
public:
	static CEquipBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};
