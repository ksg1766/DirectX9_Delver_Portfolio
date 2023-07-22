#pragma once
#include "Monster.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

END

class CBossFireWall :
    public CMonster
{
private:
	explicit	CBossFireWall(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBossFireWall(const CBossFireWall& rhs);
	virtual		~CBossFireWall();

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta)	override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

public:
	virtual void	Init_Stat()								override;

public:
	//테스트용충돌코드
	void			Set_Center(_vec3 _vPos) { m_vCenter = _vPos; }
	void			Set_Angle(_float _vAngle) { m_fAngle = _vAngle; }

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	CRcTex*			m_pBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;
	CBillBoard*		m_pBillBoard = nullptr;
	_vec3			m_vCenter;
	_float			m_fFrame;
	_float			m_fDuration;
	_float			m_fSpeed;
	_float			m_fAngle;
private:
	HRESULT		Add_Component(void);

public:
	static CBossFireWall* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

