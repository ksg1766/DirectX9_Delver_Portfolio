#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

END

class CPuzzle_Part : public Engine::CItem
{
private:
	explicit CPuzzle_Part(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPuzzle_Part(const CPuzzle_Part& rhs);
	virtual ~CPuzzle_Part();

public:
	virtual HRESULT Ready_Object(_uint _PuzzleNumber);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;


private:
	HRESULT			Add_Component(void);

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
	void	Set_PuzzleNumber(_uint _iNumber)	 { m_iPuzzleNumber = _iNumber; }
	_uint	Get_PuzzleNumber()					 { return m_iPuzzleNumber; }


private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;

	_uint	m_iPuzzleNumber;
	_int	m_iMoveTick;
	_vec3	m_vOriginPos;
public:
	static CPuzzle_Part* Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint _PuzzleNumber);

private:
	virtual void Free() override;
};

