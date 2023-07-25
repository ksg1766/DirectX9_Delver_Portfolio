#pragma once
#include "GameObject.h"
BEGIN(Engine)
class CRcTex;
class CTexture;
END
class CBoss_WarningEff :
    public CGameObject
{
private:
	explicit	CBoss_WarningEff(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBoss_WarningEff(const CBoss_WarningEff& rhs);
	virtual		~CBoss_WarningEff();
public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta)	override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;

	_float			m_fTime;
	_float			m_fScale;
private:
	HRESULT		Add_Component(void);

public:
	static CBoss_WarningEff* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

