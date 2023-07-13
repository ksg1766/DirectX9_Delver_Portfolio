#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

END

class CBoss_BatSwarm :
    public CMonster
{
private:
	explicit	CBoss_BatSwarm(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBoss_BatSwarm(const CBoss_BatSwarm& rhs);
	virtual		~CBoss_BatSwarm();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
	void	Move_to_NewPos(_vec3 _vPos, const _float& fTimeDelta);
	void	Move_to_Random(const _float& fTimeDelta);
	void	Set_StartPos(_vec3 vStartPos);
	void	Set_MovePos(_vec3 vtPos);
	void	Set_Bool(_bool _b) { m_bTest = _b; }
	void	Add_Angle(int _iAngle) { m_fAngle = (_float)_iAngle;}
private:
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;
	_vec3		m_vTargetPos;
	_vec3		m_vDir;
	_vec3		m_vMovePos;

	_float		m_fAngle;
	_float		m_fFrame;
	_float		m_fRallyTime;

	_bool		m_bTest;
private:
	HRESULT		Add_Component(void);

public:
	static CBoss_BatSwarm* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

