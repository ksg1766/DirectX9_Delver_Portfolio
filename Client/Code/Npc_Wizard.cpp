#include "stdafx.h"
#include "SoundManager.h"
#include "Npc_Wizard.h"
#include "Export_Function.h"
#include "NPC_Wizard_Idle.h"
#include "Player.h"
#include "FlyingCamera.h"

CNpc_Wizard::CNpc_Wizard(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CNpc(pGraphicDev)
{
}

CNpc_Wizard::CNpc_Wizard(const CNpc_Wizard& rhs)
	: Engine::CNpc(rhs)
{
}

CNpc_Wizard::~CNpc_Wizard()
{
}

HRESULT CNpc_Wizard::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::NPC;
	m_eNPCTag = NPCTAG::WIZARD;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Translate(_vec3(0.f, 3.f, 0.f));

	CState* pState = CNpc_Wizard_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, pState);

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::IDLE], STATE::IDLE, 8.f, TRUE);
	m_pAnimator->Add_Animation(STATE::IDLE, pAnimation);

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::IDLE);

	m_iCount = 0;
	m_iMaxCount = 2;

	m_bTalkButton = false;
	m_bTalkBoX = false;
	m_bTalking = false;
	m_pFontconfig = dynamic_cast<CFont*>(m_pFont)->Create_3DXFont(35, 20.f, 1000.f, false, L"둥근모꼴", m_pFontconfig);
	dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
	dynamic_cast<CFont*>(m_pFont)->Set_FontColor(_uint(0xffffffff));
	dynamic_cast<CFont*>(m_pFont)->Set_Rect(RECT{ 0, 350, WINCX, 650 });
	dynamic_cast<CFont*>(m_pFont)->Set_Anchor(DT_CENTER | DT_NOCLIP);

	return S_OK;
}

_int CNpc_Wizard::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_uint iExit = __super::Update_Object(fTimeDelta);

	m_pStateMachine->Update_StateMachine(fTimeDelta);

	CPlayer& rPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&vDir);

	CGameObject* pGameObject = SceneManager()->
		Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front();

	if (fDistance < 3.f)
	{
		m_bTalkButton = true;

		if (Engine::InputDev()->Key_Down(DIK_F))
		{
			if (!m_bFirstTalk)
			{
				if (Engine::UIManager()->Set_SpeechBubbleUse())
				{
					rPlayer.Set_Talk(true);
					m_bTalkBoX = true;

					if (!m_bTalking)
						m_bTalking = true;

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
				m_bTalkBoX = false;
				m_bTalking = false;
				rPlayer.Set_Talk(false);

				static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);

				CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
				CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_close.mp3", CHANNELID::SOUND_UI, 1.f);

				m_iCount = 0;
				m_bFirstTalk = false;
			}
		}
	}
	else
	{
		m_bTalkButton = false;
		m_bTalkBoX = false;
	}


	return iExit;
}

void CNpc_Wizard::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
}

void CNpc_Wizard::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();

	if (!SceneManager()->Get_GameStop())
	{
		if ((!m_bTalkBoX) && (m_bTalkButton))
		{
			dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
			m_pFont->DrawText(L"F : TALK");
		}
	}
}

void CNpc_Wizard::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (OBJECTTAG::PLAYER == _pOther->Get_ObjectTag())
	{

	}
}

void CNpc_Wizard::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CNpc_Wizard::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CNpc_Wizard::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

#pragma region 텍스쳐
	pComponent = m_pTexture[(_uint)STATE::IDLE] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_NpcWizard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);
#pragma endregion 텍스쳐

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	pComponent = m_pStateMachine = dynamic_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pFont = dynamic_cast<CFont*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Font"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::FONT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CNpc_Wizard* CNpc_Wizard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_Wizard* pInstance = new CNpc_Wizard(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CNpc_Wizard*>(pInstance);

		MSG_BOX("Trader Npc Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_Wizard::Free()
{
	__super::Free();
}
