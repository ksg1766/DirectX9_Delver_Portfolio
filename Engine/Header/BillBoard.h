#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CBillBoard :
    public CComponent
{
private:
	explicit CBillBoard();
	explicit CBillBoard(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBillBoard(const CBillBoard& rhs);
	virtual ~CBillBoard();

public:
	HRESULT			Ready_BillBoard();
	virtual void	LateUpdate_Component() override;

protected:

public:
	static	CBillBoard* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
private:
	virtual void			Free();
};

END