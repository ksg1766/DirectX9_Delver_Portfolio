#pragma once

#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CStateMachine;

END

class CBat : public CMonster
{
private:
	explicit CBat(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBat(const CBat& rhs);
	virtual ~CBat();

public:
	virtual HRESULT	Ready_Object()								override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object()							override;
	virtual void	Render_Object()								override;

public:

private:
	CRcTex*			m_pBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;
	CStateMachine*	m_pStateMachine = nullptr;

	_float m_fFrame;
	
private:
	HRESULT		Add_Component(void);


public:
	static CBat* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

