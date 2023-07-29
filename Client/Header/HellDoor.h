#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
END

class CDoorCube;

class CHellDoor : public CGameObject
{
private:
	CHellDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	CHellDoor(const CHellDoor& rhs);
	virtual ~CHellDoor();

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
	vector<CDoorCube*>& Get_HellDorVec() { return m_vecDoorCube; }
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

	_float	m_fSound;
	const _float m_fMaxVolume = 1.f;
	const _float m_fMinVolume = 0.f;
	const _float m_fMaxDistance = 120.f;


	vector<CDoorCube*> m_vecDoorCube;

public: 
	static CHellDoor* Create(LPDIRECT3DDEVICE9 pGraphicDev, CLayer*	pLayer);

private:
	virtual void Free() override;
};

