#include "stdafx.h"
#include "SoundManager.h"
#include "Npc_OldMan.h"
#include "Export_Function.h"
#include "OldMan_Idle.h"
#include "FlyingCamera.h"
#include "UIspeech_OldMan.h"
#include "Player.h"
CNpc_OldMan::CNpc_OldMan(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CNpc(pGraphicDev)
{
}

CNpc_OldMan::CNpc_OldMan(const CNpc_OldMan& rhs)
	: Engine::CNpc(rhs)
{
}

CNpc_OldMan::~CNpc_OldMan()
{
}

HRESULT CNpc_OldMan::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::NPC;
	m_eNPCTag = NPCTAG::OLD_MAN;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Translate(_vec3(0.f, 3.f, 0.f));

	CState* pState = CNpc_OldMan_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, pState);

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::IDLE], STATE::IDLE, 8.f, TRUE);
	m_pAnimator->Add_Animation(STATE::IDLE, pAnimation);

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::IDLE);

	m_iCount = 0;
	m_iMaxCount = 5;

	m_bTalkButton = false;
	m_bTalkingBox = false;
	m_bQuest =		false;
	m_bSpeech = false;
	m_pFontconfig = static_cast<CFont*>(m_pFont)->Create_3DXFont(32, 20.f, 1000.f, false, L"둥근모꼴", m_pFontconfig);
	static_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
	static_cast<CFont*>(m_pFont)->Set_FontColor(_uint(0xffffffff));
	static_cast<CFont*>(m_pFont)->Set_Rect(RECT { 0, 350, WINCX, 650 });
	static_cast<CFont*>(m_pFont)->Set_Anchor(DT_CENTER| DT_NOCLIP);

	m_fFontTime = 0.f;

	return S_OK;
}

_int CNpc_OldMan::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);
	m_fFontTime += fTimeDelta;
	m_pStateMachine->Update_StateMachine(fTimeDelta);

	CPlayer& rPlayer = *static_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	m_vPlayerPos = rPlayer.m_pTransform->m_vInfo[INFO_POS];
	m_vDir = m_vPlayerPos - m_pTransform->m_vInfo[INFO_POS];
	m_fDistance = D3DXVec3LengthSq(&m_vDir);

	CGameObject* pGameObject = SceneManager()->
		Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front();

	CInventory* PlayerInven = rPlayer.Get_Inventory();

	CItem* pItem = static_cast<CItem*>(PlayerInven->Get_IDItem(ITEMID::QUEST_ORB));

	if (m_fDistance < 3.f)
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
						m_bTalkingBox = true;
						m_bTalkButton = false;

						if (!m_bSpeech)
							m_bSpeech = true;

						static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(true);
						CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
						CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_open.mp3", CHANNELID::SOUND_UI, 1.f);
					}
					m_bFirstTalk = true;

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
					m_bTalkingBox = false;
					m_bSpeech = false;
					rPlayer.Set_Talk(false);

					static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);

					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
					CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_close.mp3", CHANNELID::SOUND_UI, 1.f);

					m_iCount = 0;
					m_bFirstTalk = false;
				}
			}
			else if (pItem != nullptr)
			{
				if (!m_bFirstTalk)
				{
					if (Engine::UIManager()->Set_SpeechBubbleUse())
					{
						rPlayer.Set_Talk(true);
						m_bTalkingBox = true;

						if (!m_bSpeech)
							m_bSpeech = true;

						static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(true);
						CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
						CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_open.mp3", CHANNELID::SOUND_UI, 1.f);
					}
					m_bFirstTalk = true;

					return iExit;
				}

				if (m_iCount < m_iMaxCount - 4)
				{
					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
					CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_open.mp3", CHANNELID::SOUND_UI, 1.f);

					++m_iCount;
				}
				else if (m_iCount >= m_iMaxCount - 4)
				{
					Engine::UIManager()->Set_SpeechBubbleUse();
					Engine::UIManager()->Hide_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
					SceneManager()->Set_GameStop(false);
					m_bTalkingBox = false;
					m_bTalkButton = false;
					m_bSpeech = false;
					rPlayer.Set_Talk(false);

					static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);

					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
					CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_close.mp3", CHANNELID::SOUND_UI, 1.f);

					m_iCount = 0;
					m_bFirstTalk = false;
				}
			}
		}
	}
	else
	{
		m_bTalkButton = false;
		m_bTalkingBox = false;
	}

	return iExit;
}

void CNpc_OldMan::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
}

void CNpc_OldMan::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();

	if (!SceneManager()->Get_GameStop())
	{
		if ((!m_bTalkingBox) && (m_bTalkButton))
		{
			static_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
			m_pFont->DrawText(L"F : TALK");
		}
	}
}

void CNpc_OldMan::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CNpc_OldMan::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CNpc_OldMan::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CNpc_OldMan::Add_Component()
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
	pComponent = m_pTexture[(_uint)STATE::IDLE] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_NpcOldMan"));
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

CNpc_OldMan* CNpc_OldMan::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_OldMan* pInstance = new CNpc_OldMan(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CNpc_OldMan*>(pInstance);

		MSG_BOX("Trader Npc Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_OldMan::Free()
{
	__super::Free();
}
