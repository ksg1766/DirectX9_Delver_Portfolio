#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

END

class CPuzzle_Part;

class CPuzzle : public Engine::CItem
{
private:
	explicit CPuzzle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPuzzle(const CPuzzle& rhs);
	virtual ~CPuzzle();

public:
	virtual HRESULT Ready_Object();
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;


private:
	HRESULT			Add_Component(void);

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);


private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;

	_bool		m_bCreate = false;

	vector<CPuzzle_Part*> m_vecPuzzlePart;

public:
	static CPuzzle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

