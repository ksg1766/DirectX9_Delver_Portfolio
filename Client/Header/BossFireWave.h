#pragma once
#include "Monster.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

END

class CBossFireWave :
    public CMonster
{
private:
	explicit	CBossFireWave(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBossFireWave(const CBossFireWave& rhs);
	virtual		~CBossFireWave();

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta)	override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

public:
	virtual void	Init_Stat()								override;

public:
	//테스트용충돌코드
	void			Set_Dir(_vec3 _vDir);
	void			Set_Angle(_float _vAngle) { m_fAngle = _vAngle; }
	void			Set_Scale(_float _fScale) {	m_fScale = _fScale; }

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	CRcTex*			m_pBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;
	_vec3			m_vDir;
	_float			m_fFrame;
	_float			m_fDuration;
	_float			m_fSpeed;
	_float			m_fAngle;
	_float			m_fScale;
private:
	HRESULT		Add_Component(void);

public:
	static CBossFireWave* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

