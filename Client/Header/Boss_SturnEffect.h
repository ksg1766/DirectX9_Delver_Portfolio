#pragma once
#include "Monster.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;
END
class CTerrain;

class CBoss_SturnEffect :
    public CMonster
{
private:
	explicit	CBoss_SturnEffect(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBoss_SturnEffect(const CBoss_SturnEffect& rhs);
	virtual		~CBoss_SturnEffect();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

public:

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;
	_float			m_fFrame;
	_float		m_fTime;
private:
	HRESULT		Add_Component(void);

public:
	static CBoss_SturnEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

