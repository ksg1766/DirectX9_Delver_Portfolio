#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CAnimator :
    public CComponent
{
private:
	explicit CAnimator();
	explicit CAnimator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimator(const CAnimator& rhs);
	virtual ~CAnimator();

public:
	HRESULT			Ready_Animator();
	virtual _int	LateUpdate_Component(const _float& fTimeDelta);

protected:
	

public:
	static	CAnimator* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
private:
	virtual void			Free();
};

END