#include "stdafx.h"
#include "SoundManager.h"
#include "Npc_Trader.h"
#include "Export_Function.h"
#include "Trader_Idle.h"
#include "Player.h"
#include "FlyingCamera.h"

CNpc_Trader::CNpc_Trader(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CNpc(pGraphicDev)
{
}

CNpc_Trader::CNpc_Trader(const CNpc_Trader& rhs)
	: Engine::CNpc(rhs)
{
}

CNpc_Trader::~CNpc_Trader()
{
}

HRESULT CNpc_Trader::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::NPC;
	m_eNPCTag = NPCTAG::TRADER;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Translate(_vec3(0.f, 3.f, 0.f));

	CState* pState = CNpc_Trader_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, pState);

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::IDLE], STATE::IDLE, 8.f, TRUE);
	m_pAnimator->Add_Animation(STATE::IDLE, pAnimation);

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::IDLE);

	m_iCount = 0;
	m_iMaxCount = 2;

	m_bTalkBox = false;
	m_bTalkButton = false;
	m_bTalking = false;
	m_bUse = false;
	m_pFontconfig = static_cast<CFont*>(m_pFont)->Create_3DXFont(35, 20.f, 1000.f, false, L"둥근모꼴", m_pFontconfig);
	static_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
	static_cast<CFont*>(m_pFont)->Set_FontColor(_uint(0xffffffff));
	static_cast<CFont*>(m_pFont)->Set_Rect(RECT{ 0, 350, WINCX, 650 });
	static_cast<CFont*>(m_pFont)->Set_Anchor(DT_CENTER | DT_NOCLIP);
	return S_OK;
}

_int CNpc_Trader::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_uint iExit = __super::Update_Object(fTimeDelta);

	m_pStateMachine->Update_StateMachine(fTimeDelta);

	CPlayer& rPlayer = *static_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&vDir);

	CGameObject* pGameObject = SceneManager()->
		Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front();

	CInventory* PlayerInven = rPlayer.Get_Inventory();

	CItem* pItem = static_cast<CItem*>(PlayerInven->Get_IDItem(ITEMID::QUEST_ORB));
	
	if (fDistance < 3.f)
	{
		m_bTalkButton = true;


		if (Engine::InputDev()->Key_Down(DIK_F))
		{

			if (pItem == nullptr)
			{
				if (!m_bFirstTalk)
				{
					if (Engine::UIManager()->Set_SpeechBubbleUse())
					{
						rPlayer.Set_Talk(true);
						m_bTalkButton = false;

						if (!m_bTalking)
							m_bTalking = true;

						static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(true);
						CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
						CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_open.mp3", CHANNELID::SOUND_UI, 1.f);
					}
					m_bFirstTalk = true;

					return iExit;
				}


				if (m_bUse)
				{
					m_bUse = false;
					rPlayer.Set_Talk(false);
					Engine::UIManager()->Set_Shop();
					static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);

					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
					CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_open.mp3", CHANNELID::SOUND_UI, 1.f);

					m_iCount = 0;
					m_bFirstTalk = false;

					return iExit;
				}

				if (m_iCount < m_iMaxCount)
				{
					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
					CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_open.mp3", CHANNELID::SOUND_UI, 1.f);

					++m_iCount;
				}
				else if (m_iCount >= m_iMaxCount)
				{
					Engine::UIManager()->Set_SpeechBubbleUse();
					Engine::UIManager()->Hide_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
					SceneManager()->Set_GameStop(false);
					m_bTalkBox = false;
					m_bTalking = false;
					rPlayer.Set_Talk(true);

					static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(true);

					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
					CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_close.mp3", CHANNELID::SOUND_UI, 1.f);

					m_bUse = true;
					Engine::UIManager()->Set_Shop();
				}


			}
			else if (pItem != nullptr)
			{
				if (Engine::UIManager()->Set_SpeechBubbleUse())
				{
					rPlayer.Set_Talk(true);
					m_bTalkButton = false;

					if (!m_bTalking)
						m_bTalking = true;

					static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(true);
					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
					CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_open.mp3", CHANNELID::SOUND_UI, 1.f);
				}
				else
				{
					Engine::UIManager()->Hide_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
					SceneManager()->Set_GameStop(false);
					m_bTalkBox = false;
					m_bTalking = false;
					rPlayer.Set_Talk(false);

					static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);

					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
					CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_close.mp3", CHANNELID::SOUND_UI, 1.f);
				}
			}
			
		}
	}
	else
	{
		m_bTalkButton = false;
		m_bTalkBox = false;
	}

	return iExit;
}

void CNpc_Trader::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
}

void CNpc_Trader::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();

	CPlayer& rPlayer = *static_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (!SceneManager()->Get_GameStop())
	{
		if ((!m_bTalkBox) && (m_bTalkButton) && !rPlayer.IsTalk() && !m_bUse)
		{
			m_pFont->Set_pFont(m_pFontconfig);
			m_pFont->DrawText(L"F : SHOP");
		}
	}
}

void CNpc_Trader::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (OBJECTTAG::PLAYER == _pOther->Get_ObjectTag())
	{
		
	}
}

void CNpc_Trader::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CNpc_Trader::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CNpc_Trader::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

#pragma region 텍스쳐
	pComponent = m_pTexture[(_uint)STATE::IDLE] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_NpcTrader"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
#pragma endregion 텍스쳐

	pComponent = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pAnimator = static_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	pComponent = m_pStateMachine = static_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pBasicStat = static_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pFont = static_cast<CFont*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Font"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::FONT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CNpc_Trader* CNpc_Trader::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_Trader* pInstance = new CNpc_Trader(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CNpc_Trader*>(pInstance);

		MSG_BOX("Trader Npc Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_Trader::Free()
{
	__super::Free();
}
