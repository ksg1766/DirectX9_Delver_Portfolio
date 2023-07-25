#pragma once
#include "GameObject.h"
BEGIN(Engine)
class CRcTex;
class CTexture;
END
class CBoss_CautionEff :
    public CGameObject
{
private:
	explicit	CBoss_CautionEff(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBoss_CautionEff(const CBoss_CautionEff& rhs);
	virtual		~CBoss_CautionEff();
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
	static CBoss_CautionEff* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

