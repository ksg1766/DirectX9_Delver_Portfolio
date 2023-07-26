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
	virtual HRESULT Ready_Object(void)						override;
	virtual _int Update_Object(const _float& fTimeDelta)	override;
	virtual void LateUpdate_Object(void)					override;
	virtual void Render_Object(void)						override;

public:
	virtual void	Init_Stat()								override;

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
	void	Set_TargetPos(_vec3 vTargetPos);//목표의 위치벡터.
	void	Set_InitialDir(_vec3 vInitialDir);//초기에 퍼져나갈 방향벡터구함
private:
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;

	_vec3		m_vTargetPos;//목표지점
	_vec3		m_vDir;//목표로의 방향벡터
	_vec3		m_vInitialDir;//처음 퍼져나갈 방향벡터
	_float		m_fFrame;

	_float		m_fMoveSpeed;//처음 나아갈 벡터로의 속도
	_float		m_fDirSpeed;//목표로 나아갈 벡터로의 속도
	_float		m_fTime;
	_float		m_fLength;

	_float		m_fMaxSpeed;//최대속도
	_bool		m_bTest;
private:
	HRESULT		Add_Component(void);

public:
	static CBoss_BatSwarm* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

