#pragma once

#include "Item.h"

BEGIN(Engine)
class CRcTex;
END

class CTempItem :
    public Engine::CItem
{
private:
	explicit CTempItem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTempItem(const CTempItem& rhs);
	virtual ~CTempItem();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	CRcTex* m_pBuffer = nullptr;

public:
	static CTempItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

