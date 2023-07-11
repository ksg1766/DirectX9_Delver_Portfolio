#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CPlayerBf;
class CTransform;
class CStateMachine;
class CPlayerStat;
class CAnimator;

END



class CTerrain;
class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

	void			Set_Terrain(CTerrain* _pCurrentTerrain);

private:
	HRESULT			Add_Component(void);
	void			Key_Input(const _float& fTimeDelta);
	void			ForceHeight(_vec3 _vPos);

public:
	const _vec3*			Get_Offset()			 { return &m_vOffset; }
	CPlayerStat*	Get_Stat()						 { return m_pStat; }
	CGameObject*	Get_Item(ITEMTAG _eItem)		 { return m_pItem[(_uint)_eItem]; }
	CGameObject*	Get_CurItem()					 { return m_pCurItem; }
	_bool			Get_ItemEquip()					 { return m_bItemEquip; }
	_bool			Get_Attack()					 { return m_bIsAttack; }

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
	void			Set_Offset(_vec3 _vOffset)					 { m_vOffset = _vOffset; }
	void			Set_ItemEquip(_bool _ItemOn)				 { m_bItemEquip = _ItemOn; }
	void			Set_CurItem(CGameObject* _pCurItem)			 { m_pCurItem = _pCurItem; }
	void			Set_Attack(_bool _bAttack)					 { m_bIsAttack = _bAttack; }

public:
	void			Add_Item(CGameObject* pItem, ITEMTAG _eItem) { m_pItem[(_uint)_eItem] = pItem; }

private:
	CPlayerBf*		m_pBuffer = nullptr;
	CPlayerStat*	m_pStat = nullptr;
	CAnimator*		m_pAnimator = nullptr;

	// 게임아이템
	CGameObject*	m_pItem[(_uint)ITEMTAG::ITEM_END];
	CGameObject*	m_pCurItem = nullptr;
	ITEMTAG			m_eItem = ITEMTAG::ITEM_END;
	_bool			m_bItemEquip;
	_bool			m_bIsAttack; // 공격형 아이템의 업데이트에 신호를 줄 불 변수


	_float			m_fSpeed = 10.f;
	

	CTerrain*		m_pTerrain;
	_vec3			m_vOffset;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

