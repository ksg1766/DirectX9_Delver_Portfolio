#pragma once
#include "Environment.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CBillBoard;

END

class CRock : public CEnvironment
{
private:
	explicit CRock(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRock(const CRock& rhs);
	virtual ~CRock();

public:
	_uint			Get_RockNumber()            { return m_iRockNumber; }
	void			Set_RockNumber(_uint _iNum) { m_iRockNumber = _iNum; }

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

private:
	HRESULT			Add_Component(void);

private:
	_uint           m_iRockNumber = 0;

public:
	static CRock*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free() override;

};

