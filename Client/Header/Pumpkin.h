#pragma once
#include "Environment.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CBillBoard;

END

class CPumpkin : public CEnvironment
{
private:
	explicit CPumpkin(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPumpkin(const CPumpkin& rhs);
	virtual ~CPumpkin();

public:
	_uint			Get_PumpkinNumber()            { return m_iPumpkinNumber; }
	void			Set_PumpkinNumber(_uint _iNum) { m_iPumpkinNumber = _iNum; }

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

private:
	HRESULT			Add_Component(void);

private:
	_uint           m_iPumpkinNumber = 0;

public:
	static CPumpkin*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free() override;

};

