#pragma once
#include "Environment.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CBillBoard;

END

class CImmortalSprite : public CEnvironment
{
private:
	explicit CImmortalSprite(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CImmortalSprite(const CImmortalSprite& rhs);
	virtual ~CImmortalSprite();

public:
	_uint			Get_SpriteNumber()            { return m_iSpriteNumber; }
	void			Set_SpriteNumber(_uint _iNum) { m_iSpriteNumber = _iNum; }

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

private:
	HRESULT			Add_Component(void);

private:
	_uint           m_iSpriteNumber = 0;

public:
	static CImmortalSprite*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free() override;

};

