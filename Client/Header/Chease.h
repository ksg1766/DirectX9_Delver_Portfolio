#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CChease : public Engine::CItem
{
private:
	explicit CChease(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CChease(const CChease& rhs);
	virtual ~CChease();

public:
	virtual HRESULT Ready_Object(_bool _Item);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

public:
	void	Use_Chease(_float _use);
	_float	Get_CheaseCount();


private:
	HRESULT			Add_Component(void);

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	CRcTex*		m_pBuffer	 = nullptr;
	CTexture*	m_pTexture = nullptr;
	CItem*		m_pItemType = nullptr;
	//CPlayer*	m_pPlayer = nullptr;
public:
	static CChease* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item);

private:
	virtual void Free() override;
};

