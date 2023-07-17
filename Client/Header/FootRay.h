#pragma once
#include "GameObject.h"

BEGIN(Engine)

END

class CFootRay :
    public CGameObject
{
private:
	explicit CFootRay(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFootRay(const CFootRay& rhs);
	virtual ~CFootRay();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
	void			Set_Host(CGameObject* _pHost) { m_pHost = _pHost; }

public:
	CGameObject*	m_pHost;	// 컴포넌트처럼 사용하게 될 GameObject;
	CGameObject*	m_pColTarget = nullptr;

private:
	HRESULT			Add_Component(void);

public:
	static CFootRay* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

