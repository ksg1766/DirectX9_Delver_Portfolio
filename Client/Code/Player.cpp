#include "stdafx.h"
#include "..\Header\Player.h"

#include "Export_Function.h"
#include "Terrain.h"

// 임시 아이템
#include "TempItem.h"
#include "Bow.h"
#include "DynamicCamera.h"

// State
#include "PlayerState_Walk.h"
#include "PlayerState_Idle.h"
#include "Player_Attack.h"

#include "UIitem.h"
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
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Get_Collider()->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(0.f, 1.f, 0.f));
	m_vOffset = _vec3(0.55f, 0.1f, 1.8f);

	// 걷기 상태 추가
	CState* pState = CPlayerState_Walk::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ROMIMG, pState);
	pState = CPlayerState_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, pState);
	pState = CPlayer_Attack::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ATTACK, pState);

	m_pStateMachine->Set_State(STATE::IDLE);

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	Key_Input(fTimeDelta);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	m_pStateMachine->Update_StateMachine(fTimeDelta);

	ForceHeight(m_pTransform->m_vInfo[INFO_POS]);

	return iExit;
}

void CPlayer::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();

	m_pStateMachine->LateUpdate_StateMachine();
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlayer::Set_Terrain(CTerrain* _pCurrentTerrain)
{
	m_pTerrain = _pCurrentTerrain;
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
	}

	if (0 != (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_Y)) && !bCameraOn)
	{
		m_pTransform->Rotate(ROT_X, D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);

		_vec3 vRigh2t;
		D3DXVec3Cross(&vRigh2t, &m_pTransform->m_vInfo[INFO_UP], &m_pTransform->m_vInfo[INFO_LOOK]);

		_matrix matRotY;

		D3DXMatrixRotationAxis(&matRotY, &m_pTransform->m_vInfo[INFO_RIGHT], D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);
		D3DXVec3TransformCoord(&m_vOffset, &m_vOffset, &matRotY);
	}

	if (Engine::InputDev()->Key_Down(DIK_1))
	{
		m_bItemEquipRight = true;
		Engine::CGameObject* pGameObject = nullptr;
		pGameObject = CTempItem::Create(m_pGraphicDev);

		//Add_Item(pGameObject, ITEMTAG::WEAPON);
		Set_CurrentEquipRight(pGameObject);

		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	}

	// UI 단축키 추가
	if (Engine::InputDev()->Key_Down(DIK_I))
	{
		if (Engine::UIManager()->Set_InvenUse())
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(false);
		}
		else
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
	}
	else if (Engine::InputDev()->Key_Down(DIK_C))
	{
		if (Engine::UIManager()->Set_StatUse())
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(false);
		}
		else
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
	}
	else if (Engine::InputDev()->Key_Down(DIK_M))
	{
		if (Engine::UIManager()->Set_MapUse()) {
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(true);
		}
		else {
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
			SceneManager()->Set_GameStop(false);
		}
	}
	else if (Engine::InputDev()->Key_Down(DIK_ESCAPE))
	{
		if (Engine::UIManager()->Set_EscUse()) {
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(true);
		}
		else
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
			SceneManager()->Set_GameStop(false);
		}
	}

	// 아이템 줍기 및 버리기
	if (Engine::InputDev()->Key_Down(DIK_E))
	{
		//  바라보고 있는 아이템 줍기 / E 키로 앞에 있는 아이템 획득을 테스트 용으로 임시 생성
		Engine::CGameObject* pGameObjectItem = CBow::Create(m_pGraphicDev);
		// 획득한 아이템 타입 및 개수를 받아옴.
		ITEMTYPEID ItemType = dynamic_cast<CItem*>(pGameObjectItem)->Get_ItemTag();

		// 장착하는 아이템 타입이고 아이템을 장착하고 있지 않다면 장착(생성)하면서 인벤토리에 획득
		if (ItemType.eItemType == ITEMTYPE_WEAPONITEM && !m_bItemEquipRight) // 오른손 장착 아이템 타입
		{
			m_bItemEquipRight = true;
			Set_CurrentEquipRight(pGameObjectItem);
			Engine::EventManager()->CreateObject(pGameObjectItem, LAYERTAG::GAMELOGIC);
		}

		// 획득한 아이템을 인벤토리에 넣는다.	
		m_pInventory->Add_ItemObject(pGameObjectItem);

		//// ITEM UI 객체 생성 후 들어온 아이템 타입 및 아이디로 값 셋팅.
		Engine::CGameObject* pGameObjectUI = CUIitem::Create(m_pGraphicDev);
		dynamic_cast<CUIitem*>(pGameObjectUI)->Set_ItemTag(ItemType.eItemType, ItemType.eItemID, ItemType.iCount);
		// 셋팅 후 UI 매니저에 아이템UI 추가.
		Engine::UIManager()->AddItemGameobject_UI(pGameObjectUI);
	}
	else if (Engine::InputDev()->Key_Down(DIK_Q))
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

				// 아이템 UI 내부에서도 해당 아이템 UI를 찾아 삭제.
				Engine::UIManager()->Delete_FindItemUI(ItemType);

				// 1개만 가지고 있었기에 버림으로 인해 보유 X, 다음 아이템을 들기 위한 초기화
				m_pCurrentEquipItemRight = nullptr;
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
}

void CPlayer::ForceHeight(_vec3 _vPos)
{
	_float x = (VTXCNTX * VTXITV / 2.f) + _vPos.x;
	_float z = (VTXCNTZ * VTXITV / 2.f) + _vPos.z;

	x /= (_float)VTXITV;
	z /= (_float)VTXITV;

	_int col = ::floorf(x);
	_int row = ::floorf(z);

	_vec3 A = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col];
	_vec3 B = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col + 1];
	_vec3 C = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col];
	_vec3 D = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col + 1];

	_float dx = x - col;
	_float dz = z - row;

	_float height;

	if (dz < 1.0f - dx)
	{
		/*
		Lerp(_float _a, _float _b, _float _c)
		{
			return a - (a * t) + (b * t);
		}
		*/

		_vec3 uy = B - A;
		_vec3 vy = C - A;

		height = A.y + (uy.y * dx) + (vy.y * dz) + 1.f;
	}
	else
	{
		_vec3 uy = C - D;
		_vec3 vy = B - D;

		height = D.y + (uy.y * (1.f - dx)) + (vy.y * (1.f - dz)) + 1.f;
	}
	_float fOffsetHeight = height - m_pTransform->m_vInfo[INFO_POS].y;
	m_pTransform->Translate(_vec3(0.f, fOffsetHeight, 0.f));
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


			}
			else // 오른 손에 장착하고 있는 상태일 시 
			{
				if (dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemTag().eItemID == ItemType.eItemID)
				{
					// 장착하려는 타입과 같은 타입을 들고 있을 시 장착 해제 및 슬롯 UI 원래 색상으로 변경

				}
				else
				{
					// 장착하려는 타입과 다른 타입을 들고 있을 시 기존 아이템 장착 해제 및 해당 아이템으로 재 장착 + UI 슬롯 밝은 색상으로 변경


				}
			}
		}
		else if (ItemType.eItemType == ITEMTYPE_GENERALITEM) // 왼 손 장착 아이템 타입
		{
			// 왼손에 장착 및 아이템 위치 손 슬롯으로 위치 이동

			if (!m_bItemEquipLeft) // 왼 손에 장착하고 있는 상태가 아닐 시 
			{
				// 왼손에 장착 및 해당 위치 슬롯으로 위치 이동


			}
			else // 왼 손에 장착하고 있는 상태일 시 
			{
				// 장착하려는 타입과 같은 타입을 들고 있을 시 장착 해제


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
	if (_pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTER && _pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTERBULLET)
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

		if (fRadiusX == fMinAxis)
		{
			if (vOtherPos.x < vThisPos.x)
				m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
			else
				m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
		}
		else if (fRadiusZ == fMinAxis)
		{
			if (vOtherPos.z < vThisPos.z)
				m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
			else
				m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
		}
		else //(fRadiusY == fMinAxis)
		{
			if (vOtherPos.y < vThisPos.y)
				m_pTransform->Translate(_vec3(0.f, fRadiusY, 0.f));
			else
				m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
		}
	}

}

void CPlayer::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTER && _pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTERBULLET)
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

		if (fRadiusX == fMinAxis)
		{
			if (vOtherPos.x < vThisPos.x)
				m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
			else
				m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
		}
		else if (fRadiusZ == fMinAxis)
		{
			if (vOtherPos.z < vThisPos.z)
				m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
			else
				m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
		}
		else //(fRadiusY == fMinAxis)
		{
			if (vOtherPos.y < vThisPos.y)
				m_pTransform->Translate(_vec3(0.f, fRadiusY, 0.f));
			else
				m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
		}
	}
	
}

void CPlayer::OnCollisionExit(CCollider* _pOther)
{
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