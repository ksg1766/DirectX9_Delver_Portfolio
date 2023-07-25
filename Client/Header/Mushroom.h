#pragma once
#include "Environment.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CBillBoard;

END

class CMushroom : public CEnvironment
{
private:
	explicit CMushroom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMushroom(const CMushroom& rhs);
	virtual ~CMushroom();

public:
	_uint			Get_MushroomNumber()            { return m_iMushroomNumber; }
	void			Set_MushroomNumber(_uint _iNum) { m_iMushroomNumber = _iNum; }

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

private:
	HRESULT			Add_Component(void);

private:
	_uint           m_iMushroomNumber = 0;

public:
	static CMushroom*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free() override;

};

