#include "stdafx.h"
#include "..\Header\Player.h"

#include "Export_Function.h"

// 임시 아이템
#include "TempItem.h"
#include "Bow.h"
#include "FireWands.h"
#include "Beer.h"
#include "Helmet.h"
#include "DynamicCamera.h"

// State
#include "PlayerState_Walk.h"
#include "PlayerState_Idle.h"
#include "Player_Attack.h"

#include "UIitem.h"
#include <UIbasicslot.h>

// 임시 몬스터 삭제용
#include "PoolManager.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CPlayer::CPlayer(const CPlayer& rhs)
	: Engine::CGameObject(rhs)
{

}

CPlayer::~CPlayer()
{
	Free();
}

HRESULT CPlayer::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::PLAYER;
	m_bItemEquipRight = false;
	m_bItemEquipLeft = false;
	m_bIsAttack = false;
	m_bAttackTick = true;
	m_bDrunk = false;
	m_bLeftRot = false;
	m_bRightRot = false;
	m_bStartRot = true;
	m_iDrunkCount = 0.f;
	m_iRootCount = 0;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(0.f, 10.f, 0.f));
	//m_vOffset	  =	_vec3(0.7f, -0.6f, 1.5f);
	//m_vLeftOffset = _vec3(-0.7, -0.6f, 1.5f);

	// 걷기 상태 추가
	CState* pState = CPlayerState_Walk::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ROMIMG, pState);
	pState = CPlayerState_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, pState);
	pState = CPlayer_Attack::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ATTACK, pState);

	m_pStateMachine->Set_State(STATE::IDLE);


#pragma region PlayerStat
	// 현재 상태
	m_pStat->Get_Stat()->fMaxHP			= 12.f;
	m_pStat->Get_Stat()->fHP			= 12.f;
	m_pStat->Get_Stat()->iDamageMin		= 1;
	m_pStat->Get_Stat()->iDamageMax		= 2;
	m_pStat->Get_Stat()->iArmorMin		= 1;
	m_pStat->Get_Stat()->iArmorMax		= 2;

	// 스텟
	m_pStat->Get_Stat()->fSpeed			= 4.f;
	m_pStat->Get_Stat()->fAgility		= 4.f;
	m_pStat->Get_Stat()->fDeffense		= 4.f;
	m_pStat->Get_Stat()->fMagic			= 4.f;
	m_pStat->Get_Stat()->fAttack		= 4.f;

	m_pStat->Get_Stat()->fHealth		= 4.f;
	m_pStat->Get_Stat()->iExp			= 0.f;
	m_pStat->Get_Stat()->iExpMax		= 8.f;
	m_pStat->Get_Stat()->iGold			= 100;
	m_pStat->Get_Stat()->iLevel			= 1;

	iDefalutDamageMax = 0;
	iDefalutDamageMin = 0;
	iArmorMax = 0;
	iArmorMin = 0;
	m_bEquipStat = false;

#pragma endregion

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	//Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	// 
	//
	Key_Input(fTimeDelta);
	if (SceneManager()->Get_GameStop()) { return 0; }
	m_pRigidBody->Update_RigidBody(fTimeDelta);

	_int iExit = __super::Update_Object(fTimeDelta);

	m_pStateMachine->Update_StateMachine(fTimeDelta);

#pragma region ksg

	if (m_pCurrentEquipItemRight)
	{
		if (!m_bIsAttack)
		{
			CTransform* pRightTrans = m_pCurrentEquipItemRight->m_pTransform;
			_vec3 vLocalScale = pRightTrans->LocalScale();

			pRightTrans->Copy_RUL(m_pTransform->m_vInfo);

			for (_int i = 0; i < INFO_POS; ++i)
				pRightTrans->m_vInfo[i] *= *(((_float*)&vLocalScale) + i);
		}
	}

	if (m_pCurrentEquipItemLeft)
	{
		CTransform* pLeftTrans = m_pCurrentEquipItemLeft->m_pTransform;
		_vec3 vLocalScale = pLeftTrans->LocalScale();

		pLeftTrans->Copy_RUL(m_pTransform->m_vInfo);

		for (_int i = 0; i < INFO_POS; ++i)
			pLeftTrans->m_vInfo[i] *= *(((_float*)&vLocalScale) + i);
	}

#pragma endregion ksg

	if (!m_bEquipStat)
	{
		iDefalutDamageMax = m_pStat->Get_Stat()->iDamageMax;
		iDefalutDamageMin = m_pStat->Get_Stat()->iDamageMin;
		iArmorMax = m_pStat->Get_Stat()->iArmorMax;
		iArmorMin = m_pStat->Get_Stat()->iArmorMin;
	}

	if (m_pCurrentEquipItemRight)
	{
		m_pStat->Get_Stat()->iDamageMax = iDefalutDamageMax + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMax;
		m_pStat->Get_Stat()->iDamageMin = iDefalutDamageMin + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMin;

		m_bEquipStat = true;
	}
	else
	{
		m_pStat->Get_Stat()->iDamageMax = iDefalutDamageMax;
		m_pStat->Get_Stat()->iDamageMin = iDefalutDamageMin;

		m_bEquipStat = false;
	}


	return iExit;
}

void CPlayer::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();

	if (Get_Drunk())
	{
		if (m_bStartRot)
		{
			m_bStartRot = false;
			D3DXMatrixIdentity(&m_matPlayerWorld);
			m_matPlayerWorld = m_pTransform->WorldMatrix();
		}

		if (m_iRootCount < 4)
		{
			if (m_iDrunkCount < 30 && !m_bRightRot)
			{
				++m_iDrunkCount;
				m_pTransform->Rotate(ROT_Z, 0.05 *  Engine::Get_TimeDelta(L"Timer_FPS60"));
			}
			else if (m_iDrunkCount > -30 && !m_bLeftRot)
			{
				--m_iDrunkCount;
				m_pTransform->Rotate(ROT_Z, -0.05  * Engine::Get_TimeDelta(L"Timer_FPS60"));
				m_bRightRot = true;
			}
			else
			{
				m_bRightRot = false;
				++m_iRootCount;
			}
		}
		else if (m_iDrunkCount == -20 && !m_bRightRot)
		{
			m_pTransform->Set_WorldMatrix(m_matPlayerWorld);
			Set_Drunk(false);
			m_bStartRot = true;
			m_bLeftRot = false;
			m_bRightRot = false;
		}
	}

	m_pStateMachine->LateUpdate_StateMachine();
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CPlayerBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_PlayerBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pStateMachine = dynamic_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Player_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	pComponent = m_pStat = dynamic_cast<CPlayerStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Player_Stat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	pComponent = m_pInventory = dynamic_cast<CInventory*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Inventory"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::INVENTORY, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	CGameObject* pGameObject = SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front();

	_long dwMouseMove;

	_vec3 vLook = m_pTransform->m_vInfo[INFO_LOOK];
	_vec3 vRight = m_pTransform->m_vInfo[INFO_RIGHT];
	_vec3 vUp = m_pTransform->m_vInfo[INFO_UP];

	_bool bCameraOn = static_cast<CDynamicCamera*>(pGameObject)->Get_MouseFix();

	if (0 != (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_X)) && !bCameraOn)
	{
		_matrix matRotX;

		m_pTransform->Rotate(ROT_Y, D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);

		D3DXMatrixRotationAxis(&matRotX, &m_pTransform->m_vInfo[INFO_UP], D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);
		D3DXVec3TransformCoord(&m_vOffset, &m_vOffset, &matRotX);
		D3DXVec3TransformCoord(&m_vLeftOffset, &m_vLeftOffset, &matRotX);
	}

	if (0 != (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_Y)) && !bCameraOn)
	{
		m_pTransform->Rotate(ROT_X, D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);

		_vec3 vRigh2t;
		D3DXVec3Cross(&vRigh2t, &m_pTransform->m_vInfo[INFO_UP], &m_pTransform->m_vInfo[INFO_LOOK]);

		_matrix matRotY;

		D3DXMatrixRotationAxis(&matRotY, &m_pTransform->m_vInfo[INFO_RIGHT], D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);
		D3DXVec3TransformCoord(&m_vOffset, &m_vOffset, &matRotY);
		D3DXVec3TransformCoord(&m_vLeftOffset, &m_vLeftOffset, &matRotY);
	}

	if (Engine::InputDev()->Key_Down(DIK_SPACE))
	{
		if (!m_IsJump)
		{
			m_pRigidBody->Add_Force(_vec3(0.f, 1.1f * m_fSpeed, 0.f));
			m_pRigidBody->UseGravity(true);
			//m_IsJump = true;
		}
	}

	// UI 단축키 추가
	if (Engine::InputDev()->Key_Down(DIK_I))
	{
		if (Engine::UIManager()->Set_InvenUse())
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(false);
			Set_UseUI(true);
			// 사용자 마우스 못 받게.
		}
		else
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
			Set_UseUI(false);
		}
	}
	else if (Engine::InputDev()->Key_Down(DIK_C))
	{
		if (Engine::UIManager()->Set_StatUse())
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(false);
			Set_UseUI(true);
		}
		else
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
			Set_UseUI(false);
		}
		
	}
	else if (Engine::InputDev()->Key_Down(DIK_M))
	{
		if (Engine::UIManager()->Set_MapUse()) {
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(true);
			Set_UseUI(true);
		}
		else {
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
			SceneManager()->Set_GameStop(false);
			Set_UseUI(false);
		}
	}
	else if (Engine::InputDev()->Key_Down(DIK_ESCAPE))
	{
		if (Engine::UIManager()->Set_EscUse()) {
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(true);
			Set_UseUI(true);
		}
		else
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
			SceneManager()->Set_GameStop(false);
			Set_UseUI(false);
		}
	}

	// 아이템 줍기 및 버리기
	//if (Engine::InputDev()->Key_Down(DIK_E))
	//{
	//	//  바라보고 있는 아이템 줍기 / E 키로 앞에 있는 아이템 획득을 테스트 용으로 임시 생성
	//	Engine::CGameObject* pGameObjectItem = CBeer::Create(m_pGraphicDev, false);
	//	//Engine::CGameObject* pGameObjectItem = CTempItem::Create(m_pGraphicDev, false);
	//	// 획득한 아이템 타입 및 개수를 받아옴.
	//	ITEMTYPEID ItemType = dynamic_cast<CItem*>(pGameObjectItem)->Get_ItemTag();

	//	// 획득한 아이템을 인벤토리에 넣는다.	
	//	m_pInventory->Add_ItemObject(pGameObjectItem);

	//	//// ITEM UI 객체 생성 후 들어온 아이템 타입 및 아이디로 값 셋팅.
	//	Engine::CGameObject* pGameObjectUI = CUIitem::Create(m_pGraphicDev);
	//	dynamic_cast<CUIitem*>(pGameObjectUI)->Set_ItemTag(ItemType.eItemType, ItemType.eItemID, ItemType.iCount);
	//	// 셋팅 후 UI 매니저에 아이템UI 추가.
	//	Engine::UIManager()->AddItemGameobject_UI(pGameObjectUI);

	//	// 장착하는 아이템 타입이고 아이템을 장착하고 있지 않다면 장착(생성)
	//	if (ItemType.eItemType == ITEMTYPE::ITEMTYPE_GENERALITEM && !m_bItemEquipLeft) // 오른손 장착 아이템 타입
	//	{
	//		m_bItemEquipLeft = true;
	//		Set_CurrentEquipLeft(pGameObjectItem);
	//		Set_PrevEquipLeft(pGameObjectItem);
	//		Engine::EventManager()->CreateObject(pGameObjectItem, LAYERTAG::GAMELOGIC);

	//		// 장착 상태 // 해당 아이템 찾아서 해당 슬롯 장착 상태 표시
	//		Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
	//		dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);
	//	}
	//}
	if (Engine::InputDev()->Key_Down(DIK_Q))
	{
		// 오른손에 들고 있는 아이템만 Q키로 버리기 가능

		if (m_bItemEquipRight == true) // 오른손에 아이템을 장착하고 있는 상태일 시 
		{
			// 오른손에 장착하고 있는 아이템 타입을 가져옴.
			ITEMTYPEID ItemType = dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemTag();

			// 해당 아이템의 개수가 1개일시와 그 이상일 시를 나눔 : 1개면 아이템을 버리면서 오른 손에 장착하고 있던 정보 초기화, 1개 이상이면 해당 아이템 카운트 1감소(장착 유지)
			if (ItemType.iCount == 1)
			{
				// 인벤토리 내에서 해당 아이템을 찾아 삭제.
				m_pInventory->delete_FindItem(ItemType);

				// 장착 상태 해제
				Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(false);

				// 아이템 UI 내부에서도 해당 아이템 UI를 찾아 삭제.
				Engine::UIManager()->Delete_FindItemUI(ItemType);

				// 1개만 가지고 있었기에 버림으로 인해 보유 X, 다음 아이템을 들기 위한 초기화
				m_pCurrentEquipItemRight = nullptr;
				m_pPrevEquipItemRight = nullptr;
				m_bItemEquipRight = false;
			}
			else if (ItemType.iCount > 1)
			{
				ItemType.iCount = 1; // 여러개를 보유하고 있기에 손에 들고있는 1개씩 버린다.

				// 인벤토리 내에서 해당 아이템을 찾아 삭제.
				m_pInventory->delete_FindItem(ItemType);

				//아이템 UI 내부에서도 해당 아이템을 찾아 삭제.
				Engine::UIManager()->Delete_FindItemUI(ItemType);
			}
		}
	}

	// 1 2 3 4 5 슬롯에 있는 아이템 사용(소멸 되는 것) 및 장착 + 해제하기
	if (Engine::InputDev()->Key_Down(DIK_F1))
	{
		Use_SlotItem(KEYSLOT_ONE);
	}
	else if (Engine::InputDev()->Key_Down(DIK_F2))
	{
		Use_SlotItem(KEYSLOT_TWO);
	}
	else if (Engine::InputDev()->Key_Down(DIK_F3))
	{
		Use_SlotItem(KEYSLOT_THREE);
	}
	else if (Engine::InputDev()->Key_Down(DIK_F4))
	{
		Use_SlotItem(KEYSLOT_FOUR);
	}
	else if (Engine::InputDev()->Key_Down(DIK_F5))
	{
		Use_SlotItem(KEYSLOT_FIVE);
	}

	if (Engine::InputDev()->Key_Down(DIK_DELETE))
	{
		vector<CGameObject*>& vecMonsterList = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::MONSTER);
		if (!vecMonsterList.empty())
			CPoolManager::GetInstance()->Delete_Object(vecMonsterList.back());
	}
}

void CPlayer::Use_SlotItem(INVENKEYSLOT _SlotNum)
{
	CGameObject* SlotItemObj = m_pInventory->Get_KeySlotObject(_SlotNum);

	if (SlotItemObj != nullptr) {
		ITEMTYPEID ItemType = dynamic_cast<CItem*>(SlotItemObj)->Get_ItemTag();
		if (ItemType.eItemType == ITEMTYPE_WEAPONITEM)        // 오른 손 장착 아이템 타입
		{
			if (!m_bItemEquipRight) // 오른 손에 장착하고 있는 상태가 아닐 시 
			{
				// 오른손에 장착 및 해당 위치 슬롯 UI 배경 밝은 색상으로 변겅
				m_bItemEquipRight = true;
				
				Set_CurrentEquipRight(SlotItemObj);
				Set_PrevEquipRight(SlotItemObj);

				Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);
			}
			else // 오른 손에 장착하고 있는 상태일 시 
			{
				if (dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemTag().eItemID == ItemType.eItemID)
				{
					// 장착하려는 타입과 같은 타입을 들고 있을 시 장착 해제 및 슬롯 UI 원래 색상으로 변경
					m_bItemEquipRight = false;
					Set_CurrentEquipRight(nullptr);
					Set_PrevEquipRight(SlotItemObj);
					Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(false);
				}
				else
				{
					// 장착하려는 타입과 다른 타입을 들고 있을 시 기존 아이템 장착 해제 및 해당 아이템으로 재 장착 + UI 슬롯 밝은 색상으로 변경
					m_bItemEquipRight = true;
					Set_CurrentEquipRight(SlotItemObj);
					ITEMTYPEID PrevItemType = dynamic_cast<CItem*>(Get_PrevEquipRight())->Get_ItemTag(); // 이전 무기의 아이템 태그갖고옴.
					Engine::CGameObject* PrevFindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(PrevItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(PrevFindSlotObj))->Set_FindSlot(false);

					Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);

					Set_PrevEquipRight(SlotItemObj);
				}
			}
		}
		else if (ItemType.eItemType == ITEMTYPE_GENERALITEM) // 왼 손 장착 아이템 타입
		{
			// 왼손에 장착 및 아이템 위치 손 슬롯으로 위치 이동

			if (!m_bItemEquipLeft) // 왼 손에 장착하고 있는 상태가 아닐 시 
			{
				// 왼손에 장착 및 해당 위치 슬롯으로 위치 이동
				m_bItemEquipLeft = true;

				Set_CurrentEquipLeft(SlotItemObj);
				Set_PrevEquipLeft(SlotItemObj);

				Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);

			}
			else // 왼 손에 장착하고 있는 상태일 시 
			{
				// 장착하려는 타입과 같은 타입을 들고 있을 시 장착 해제
				if (dynamic_cast<CItem*>(m_pCurrentEquipItemLeft)->Get_ItemTag().eItemID == ItemType.eItemID)
				{
					// 장착하려는 타입과 같은 타입을 들고 있을 시 장착 해제 및 슬롯 UI 원래 색상으로 변경
					m_bItemEquipLeft = false;
					Set_CurrentEquipLeft(nullptr);
					Set_PrevEquipLeft(SlotItemObj);
					Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(false);
				}
				else
				{
					// 장착하려는 타입과 다른 타입을 들고 있을 시 기존 아이템 장착 해제 및 해당 아이템으로 재 장착 + UI 슬롯 밝은 색상으로 변경
					m_bItemEquipLeft = true;
					Set_CurrentEquipRight(SlotItemObj);
					ITEMTYPEID PrevItemType = dynamic_cast<CItem*>(Get_PrevEquipLeft())->Get_ItemTag(); // 이전 무기의 아이템 태그갖고옴.
					Engine::CGameObject* PrevFindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(PrevItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(PrevFindSlotObj))->Set_FindSlot(false);

					Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);

					Set_PrevEquipLeft(SlotItemObj);
				}
				// 장착하려는 타입과 다른 타입을 들고 있을 시 장착 해제 및 해당 아이템으로 재 장착


			}
		}
		else if (ItemType.eItemType == ITEMTYPE_EQUIPITEM)   // 아이템 슬롯 장착 아이템 타입
		{
			// 해당 아이템 슬롯에 장착 및 위치 이동 + 장착 및 해제에 따른 효과 감소 및 증가
		}
		else if (ItemType.eItemType == ITEMTYPE_EATITEM)    // HP 회복 아이템 타입
		{
			// 해당 아이템의 회복 값에 따른 HP 회복 및 아이템 소멸

			// 아이템 효과 적용

			// 아이템 소멸
		}
		else if (ItemType.eItemType == ITEMTYPE_POTIONITEM) // 다양한 포션 아이템 타입
		{
			// 해당 아이템 능력 및 효과 적용 후 아이템 소멸

			// 아이템 효과 적용

			// 아이템 소멸
		}
	}
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTER 
		&& _pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTERBULLET
		&& _pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::ITEM)
	{
		_vec3	vOtherPos = _pOther->GetCenterPos();
		_float* fOtherAxis = _pOther->GetAxisLen();

		_vec3	vThisPos = m_pCollider->GetCenterPos();
		_float* fThisAxis = m_pCollider->GetAxisLen();

		// OBJECTTAG에 따른 예외 처리 가능성
		_float fWidth = fabs(vOtherPos.x - vThisPos.x);
		_float fHeight = fabs(vOtherPos.y - vThisPos.y);
		_float fDepth = fabs(vOtherPos.z - vThisPos.z);

		_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
		_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
		_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

		_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

		if (fRadiusY == fMinAxis)
		{
			if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER)
				return;
			if (vOtherPos.y < vThisPos.y)
			{
				//m_IsJump = false;
				//m_pRigidBody->UseGravity(false);
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));
				m_pTransform->Translate(_vec3(0.f, fRadiusY - 0.000001f, 0.f));
			}
			else
			{
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));
				m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
			}
		}
		else if (fRadiusZ == fMinAxis)
		{
			if (vOtherPos.z < vThisPos.z)
				m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
			else
				m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
		}
		else //(fRadiusX == fMinAxis)
		{
			if (vOtherPos.x < vThisPos.x)
				m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
			else
				m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
		}
	}

	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::ITEM)
	{
		if (dynamic_cast<CItem*>(_pOther->GetHost())->Get_WorldItem())
		{
			ITEMTYPEID ItemType = dynamic_cast<CItem*>(_pOther->GetHost())->Get_ItemTag();

			CGameObject* pItem = nullptr;

			switch (ItemType.eItemID)
			{
			case ITEMID::WEAPON_SWORD:
				pItem = CTempItem::Create(m_pGraphicDev, false);
				break;
			case ITEMID::WEAPON_BOW:
				pItem = CBow::Create(m_pGraphicDev, false);
				break;
			case ITEMID::WEAPON_WAND3:
				pItem = CFireWands::Create(m_pGraphicDev, false);
				break;
			case ITEMID::EQUIP_OLDHELMET:
				pItem = CHelmet::Create(m_pGraphicDev, false);
				break;
			}

			m_pInventory->Add_ItemObject(pItem);

			Engine::CGameObject* pGameObjectUI = CUIitem::Create(m_pGraphicDev);
			dynamic_cast<CUIitem*>(pGameObjectUI)->Set_ItemTag(ItemType.eItemType, ItemType.eItemID, ItemType.iCount);

			Engine::UIManager()->AddItemGameobject_UI(pGameObjectUI);

			if (ItemType.eItemType == ITEMTYPE_WEAPONITEM && !m_bItemEquipRight)
			{
				m_bItemEquipRight = true;
				
				Set_CurrentEquipRight(pItem);
				Set_PrevEquipRight(pItem);
				Engine::EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);
				Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);
			}
			else
			{
				Engine::EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);
				//Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				//dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(false);
			}


			EventManager()->GetInstance()->DeleteObject(dynamic_cast<CItem*>(_pOther->GetHost()));
		}
	}
}

void CPlayer::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTER 
		&& _pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTERBULLET
		&& _pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::ITEM)
	{
		_vec3	vOtherPos = _pOther->GetCenterPos();
		_float* fOtherAxis = _pOther->GetAxisLen();

		_vec3	vThisPos = m_pCollider->GetCenterPos();
		_float* fThisAxis = m_pCollider->GetAxisLen();

		// OBJECTTAG에 따른 예외 처리 가능성
		_float fWidth = fabs(vOtherPos.x - vThisPos.x);
		_float fHeight = fabs(vOtherPos.y - vThisPos.y);
		_float fDepth = fabs(vOtherPos.z - vThisPos.z);

		_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
		_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
		_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

		_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

		if (fRadiusY == fMinAxis)
		{
			if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER)
				return;
			if (vOtherPos.y < vThisPos.y)
			{
				//m_IsJump = false;
				//m_pRigidBody->UseGravity(false);
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));
				m_pTransform->Translate(_vec3(0.f, fRadiusY - 0.000001f, 0.f));
			}
			else
				m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
		}
		else if (fRadiusZ == fMinAxis)
		{
			if (vOtherPos.z < vThisPos.z)
				m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
			else
				m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
		}
		else //(fRadiusX == fMinAxis)
		{
			if (vOtherPos.x < vThisPos.x)
				m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
			else
				m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
		}
	}
	
}

void CPlayer::OnCollisionExit(CCollider* _pOther)
{
	//if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::BLOCK)
	//{
	//	_vec3 vThisPos = m_pTransform->m_vInfo[INFO_POS];
	//	_vec3 vOtherPos = _pOther->Get_Transform()->m_vInfo[INFO_POS];

	//	if (1.5f < vThisPos.y - vOtherPos.y && 2.5f > vThisPos.y - vOtherPos.y)
	//	{
	//		//m_IsJump = true;
	//		//m_pRigidBody->UseGravity(true);
	//	}
	//}
}

void CPlayer::IsAttack(CBasicStat* _MonsterStat)
{
	_int iDamage = 1 + rand() % (m_pStat->Get_Stat()->iDamageMin +
		(m_pStat->Get_Stat()->iDamageMax - m_pStat->Get_Stat()->iDamageMin));

	_int iDeffence = 1 + rand() % (_MonsterStat->Get_Stat()->iArmorMin + (_MonsterStat->Get_Stat()->iArmorMax - _MonsterStat->Get_Stat()->iArmorMin));

	_int iResultDamage = iDamage - iDeffence;

	if (iResultDamage <= 0)
		iResultDamage = 1;

	cout << iResultDamage << " 데미지" << endl;

	Set_AttackTick(true);
	_MonsterStat->Take_Damage(iResultDamage);
}

void CPlayer::Free()
{
	__super::Free();
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}