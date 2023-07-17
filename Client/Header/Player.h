#pragma once

#include "GameObject.h"
BEGIN(Engine)

class CPlayerBf;
class CTransform;
class CRigidBody;
class CStateMachine;
class CPlayerStat;
class CAnimator;
class CInventory;

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

private:
	HRESULT			Add_Component(void);
	void			Key_Input(const _float& fTimeDelta);
	void            Use_SlotItem(INVENKEYSLOT _SlotNum);

public:

	const _vec3*	Get_Offset()			{ return &m_vOffset; }
	const _vec3*	Get_LeftOffset()		{ return &m_vLeftOffset; }
	CPlayerStat*	Get_Stat()				{ return m_pStat; }
	//CGameObject*	Get_Item(ITEMTAG _eItem){ return m_pItem[(_uint)_eItem]; }
	CGameObject*	Get_CurrentEquipRight()	{ return m_pCurrentEquipItemRight; }
	CGameObject*    Get_CurrentEquipLeft()  { return m_pCurrentEquipItemLeft; }
	CGameObject*	Get_PrevEquipRight()	{ return m_pPrevEquipItemRight; }
	CGameObject*	Get_PrevEquipLeft()		{ return m_pPrevEquipItemLeft; }
	_bool			Get_ItemEquipRight()    { return m_bItemEquipRight; }
	_bool			Get_ItemEquipLeft()     { return m_bItemEquipLeft; }
	_bool			Get_Attack()			{ return m_bIsAttack; }
	_bool			Get_AttackTick()		{ return m_bAttackTick; }
	_bool			Get_UseUI()				{ return m_bUseUI; }
	_bool			Get_Drunk()				{ return m_bDrunk; }

	_float			Get_Speed()				{ return m_fSpeed; }
	_bool			IsJump()				{ return m_IsJump; }
	CRigidBody*		Get_RigidBody()			{ return m_pRigidBody; }

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
	void			Set_Offset(_vec3 _vOffset)						{ m_vOffset = _vOffset; }
	void			Set_ItemEquipRight(_bool _ItemOn)				{ m_bItemEquipRight = _ItemOn; }
	void			Set_CurrentEquipRight(CGameObject* _pCurItem)	{ m_pCurrentEquipItemRight = _pCurItem; }
	void			Set_ItemEquipLeft(_bool _ItemOn)				{ m_bItemEquipLeft = _ItemOn; }
	void			Set_CurrentEquipLeft(CGameObject* _pCurItem)	{ m_pCurrentEquipItemLeft = _pCurItem; }
	void			Set_Attack(_bool _bAttack)						{ m_bIsAttack = _bAttack; }
	void			Set_AttackTick(_bool _bTick)					{ m_bAttackTick = _bTick; }
	void			Set_State(STATE _eState)						{ m_eState = _eState; }
	void			Set_UseUI(_bool _use)							{ m_bUseUI = _use; }
	void			Set_PrevEquipRight(CGameObject* _pPrevItem)		{ m_pPrevEquipItemRight = _pPrevItem; }
	void			Set_PrevEquipLeft(CGameObject* _pPrevItem)		{ m_pPrevEquipItemLeft = _pPrevItem; }
	void			Set_LeftOffset(_vec3 _vOffset)					{ m_vLeftOffset = _vOffset; }
	void			Set_Drunk(_bool _Drunk)							{ m_bDrunk = _Drunk; }

	// ksg
	void			Set_JumpState(_bool _bJump)						{ m_IsJump = _bJump; }

public:
	//void			Add_Item(CGameObject* pItem, ITEMTAG _eItem) { m_pItem[(_uint)_eItem] = pItem; }

private:
	CRigidBody*		m_pRigidBody = nullptr;
	CStateMachine*	m_pStateMachine = nullptr;
	CPlayerBf*		m_pBuffer = nullptr;
	CPlayerStat*	m_pStat = nullptr;
	CAnimator*		m_pAnimator = nullptr;
	CInventory*     m_pInventory = nullptr;
	_matrix			m_matPlayerWorld;
	// 게임아이템

	//CGameObject*	m_pItem[(_uint)ITEMTAG::ITEM_END];      //-> 인벤토리로 이동
	_bool			m_bItemEquipRight;                      // 플레이어의 들고있는 아이템 장착 여부 : 오른손
	_bool			m_bItemEquipLeft;                       // 플레이어의 들고있는 아이템 장착 여부 : 왼손
	CGameObject*	m_pCurrentEquipItemRight = nullptr;     // 현재 플레이어가 들고 있는 아이템     : 오른손
	CGameObject*	m_pPrevEquipItemRight = nullptr;		// 이전 플레이어가 들고 있는 아잉템		: 오른손
	CGameObject*    m_pCurrentEquipItemLeft = nullptr;      // 현재 플레이어가 들고 있는 아이템     : 왼손
	CGameObject*	m_pPrevEquipItemLeft = nullptr;			// 이전 플레이어가 들고 있는 아이템		: 왼손
	//ITEMTAG		m_eItem = ITEMTAG::ITEM_END;


	// Player 
	STATE			m_eState = STATE::STATE_END;

	_bool			m_bUseUI;
	_bool			m_bIsAttack;  // 공격형 아이템의 업데이트에 신호를 줄 불 변수
	_bool			m_bAttackTick;
	_bool			m_bDrunk;

	_int			m_iDrunkCount;
	_float			m_fSpeed = 10.f;
	_bool			m_IsJump = false;

	_bool			m_OnGround = false;
	_bool			m_bLeftRot;
	_bool			m_bRightRot;
	_bool			m_bStartRot;

	_int			m_iRootCount;

	// Debuff
	_float			m_fDrunkTime;
	_float			m_fDrunkDuration;
	_float			m_fRotationAngle;
	_float			m_fRotationSpeed;
	_float			m_fAmount;
	_bool			m_bFoward;

	_vec3			m_vOffset;
	_vec3			m_vLeftOffset;


public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

