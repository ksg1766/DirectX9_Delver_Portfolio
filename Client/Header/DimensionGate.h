#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
END

class CDimensionGate :
    public CGameObject
{
private:
	explicit		CDimensionGate(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit		CDimensionGate(const CDimensionGate& rhs);
	virtual			~CDimensionGate();
public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta)	override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	CRcTex*			m_pBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;

	_float			m_fTime;
	_float			m_fScale;
	_float			m_fFrame;
private:
	HRESULT			Add_Component(void);

public:
	static CDimensionGate* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void	Free() override;


};

