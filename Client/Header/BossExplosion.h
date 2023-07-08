#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;
END
class CTerrain;

class CBossExplosion :
    public CGameObject
{
private:
	explicit	CBossExplosion(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CBossExplosion(const CBossExplosion& rhs);
	virtual		~CBossExplosion();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

public:
	virtual void	Set_StartPos(_vec3 _vec);
	virtual void	Set_StartPosY(float _fY);
private:
	CRcTex* m_pBuffer = nullptr;
	CTransform* m_pTransform = nullptr;
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;
	_float			m_fFrame;
	_uint		m_iCount;
private:
	HRESULT		Add_Component(void);

public:
	static CBossExplosion* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

