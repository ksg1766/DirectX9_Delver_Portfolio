#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
END

class CBookCube;

class CBookDoor : public CGameObject
{
private:
	CBookDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	CBookDoor(const CBookDoor& rhs);
	virtual ~CBookDoor();

public:
	virtual HRESULT	Ready_Object(CLayer* pLayer);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

public:
	vector<CBookCube*>& Get_FrontDoor() { return m_vecFrontDoorCube; }
	vector<CBookCube*>& Get_BackDoor()	{ return m_vecBackDoorCube; }

	_bool				Get_bTrigger() { return m_bTriger; }
private:
	HRESULT	Add_Component();

private:
	CCubeBf* m_pCubeBf = nullptr;
	CTexture* m_pTexture = nullptr;
	_bool	  m_bTriger = false;
	_bool	  m_bShake = false;
	_bool	  m_bCreate = false;
	_bool	  m_bBgmChange = false;
	_bool	  m_bBgmStop = false;
	_bool	  m_bDoorOpen = false;
	_bool	  m_bDoorClose = false;
	
	_bool	  m_bFrontOpen = false;

	_float	m_fSound;
	const _float m_fMaxVolume = 1.f;
	const _float m_fMinVolume = 0.f;
	const _float m_fMaxDistance = 4.f;


	vector<CBookCube*> m_vecFrontDoorCube;
	vector<CBookCube*> m_vecBackDoorCube;
	
	_float m_fFinalLeft = 0.f;
	_float m_fFinalRight = 0.f;
	_float m_fFinalUp = 0.f;
	_float m_fFinalDown = 0.f;

public:
	static CBookDoor* Create(LPDIRECT3DDEVICE9 pGraphicDev, CLayer* pLayer);

private:
	virtual void Free() override;
};

