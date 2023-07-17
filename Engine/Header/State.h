#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStateMachine;

class ENGINE_DLL CState : public CBase
{
protected:
	explicit CState();
	explicit CState(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CState();

public:
	virtual HRESULT		Ready_State(CStateMachine* pOwner)		PURE;

	// 상태의 타입을 반환함.
	virtual STATE		Update_State(const _float& fTimeDelta)	PURE;
	virtual	void		LateUpdate_State()						PURE;
	virtual void		Render_State()							PURE;


public:
	virtual	STATE		Key_Input(const _float& fTimeDelta)		PURE;
	
public:
	virtual STATE		Get_State()		{ return m_eState; }
	virtual STATE		Get_PrevState() { return m_ePrevState; }

	_bool	Get_AttackCool()			{ return m_bAttackCool; }
	void	Set_AttackCool(_bool _Cool) { m_bAttackCool = _Cool; }

protected:
	// 어떤 상태머신이 자신을 지니고 있는지 알려줌
	CStateMachine* m_pOwner = nullptr;
	// 어떤 상태인지 정해줌
	STATE		   m_eState = STATE::STATE_END;
	STATE		   m_ePrevState = STATE::STATE_END;

	// 스테이트 내부에서 공통적으로 사용할 변수
	_float		   m_fChase;
	_vec3		   m_vRandomPos;

	_bool		   m_bAttackCool;
	
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	

public:
	//static	CState* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	//virtual CComponent* Clone(void);

protected:
	virtual void			Free();
};

END