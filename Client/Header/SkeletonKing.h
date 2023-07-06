#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CSkeletonKing:public Engine::CGameObject
{
private:
	explicit	CSkeletonKing(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CSkeletonKing(const CSkeletonKing& rhs);
	virtual		~CSkeletonKing();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
public:

private:
	CRcTex* m_pRcBf;

private:
	HRESULT		Add_Component(void);
//	void		Key_Input(const _float& fTimeDelta);

public:
	static CSkeletonKing* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

