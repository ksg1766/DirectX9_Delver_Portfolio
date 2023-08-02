#include "stdafx.h"
#include "SoundManager.h"
#include "Phantom.h"
#include "Phantom_Idle.h"
#include "Export_Function.h"
#include "Player.h"
#include "FlyingCamera.h"
#include "Puzzle_Part.h"
#include "UIPuzzleBack.h"

CPhantom::CPhantom(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CNpc(pGraphicDev)
{
}

CPhantom::CPhantom(const CPhantom& rhs)
	: Engine::CNpc(rhs)
{
}

CPhantom::~CPhantom()
{
}

HRESULT CPhantom::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::NPC;
	m_eNPCTag = NPCTAG::PAHNTOM;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	

	m_pTransform->Scale(_vec3(1.f, 1.f, 5.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale());
	m_pTransform->Translate(_vec3(-69.f, 12.f, -10.f));

	CState* pState = CPhantom_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, pState);
	
	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::IDLE], STATE::IDLE, 8.f, TRUE);
	m_pAnimator->Add_Animation(STATE::IDLE, pAnimation);

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::IDLE);

	m_vPushPos = _vec3(-81.f, 12.f, -11.f);

	m_iCount = 0;
	m_iMaxCount = 8;

	m_pFontconfig = dynamic_cast<CFont*>(m_pFont)->Create_3DXFont(35, 20.f, 1000.f, false, L"둥근모꼴", m_pFontconfig);
	dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
	dynamic_cast<CFont*>(m_pFont)->Set_FontColor(_uint(0xffffffff));
	dynamic_cast<CFont*>(m_pFont)->Set_Rect(RECT{ 0, 350, WINCX, 650 });
	dynamic_cast<CFont*>(m_pFont)->Set_Anchor(DT_CENTER | DT_NOCLIP);

	return S_OK;
}

_int CPhantom::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_uint iExit = __super::Update_Object(fTimeDelta);

	//ForceHeight(m_pTransform->m_vInfo[INFO_POS]);
	m_pStateMachine->Update_StateMachine(fTimeDelta);

	CPlayer& rPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&vDir);

	CGameObject* pGameObject = SceneManager()->
		Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front();

	// TODO
	// 퀘스트 클리어가 안 된 상태라면 
	if (!m_bQuestClear)
	{
		if (m_bPush)
		{
			_vec3 vDir = m_vPushPos - rPlayer.m_pTransform->m_vInfo[INFO_POS];
			_float fDistance = D3DXVec3Length(&vDir);
			D3DXVec3Normalize(&vDir, &vDir);

			//m_pTransform->m_vInfo[INFO_POS] += vDir * 0.5f;
			rPlayer.m_pTransform->Translate(vDir * fTimeDelta * 10.f);


			if (fDistance <= 0.3f)
			{
				m_bPush = false;
				m_bShake = false;
			}
				
		}

		if (!m_bFirstCollision)
		{
			if (Engine::UIManager()->Set_SpeechBubbleUse())
			{

				rPlayer.Set_Talk(true);

				if (!m_bTalking)
					m_bTalking = true;

				static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(true);
				CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
				CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_open.mp3", CHANNELID::SOUND_UI, 1.f);
			}

			m_bFirstCollision = true;
		}

		CGameObject* pGameObject = SceneManager()->
			Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front();

		if (!m_bQusetStart)
		{
			if (Engine::InputDev()->Key_Down(DIK_F))
			{
				if (m_iCount < m_iMaxCount - 3)
				{
					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
					CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_open.mp3", CHANNELID::SOUND_UI, 1.f);

					++m_iCount;
				}
				else if (m_iCount >= m_iMaxCount - 3)
				{
					Engine::UIManager()->Set_SpeechBubbleUse();
					Engine::UIManager()->Hide_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
					SceneManager()->Set_GameStop(false);
					m_bTalkingBox = false;
					m_bTalking = false;
					rPlayer.Set_Talk(false);

					static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);

					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
					CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_close.mp3", CHANNELID::SOUND_UI, 1.f);

					m_bQusetStart = true;
					Create_Puzzle();


					++m_iCount;
				}
			}
		}
	}
	else if (m_bQuestClear)
	{
		_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
		_float fDistance = D3DXVec3Length(&vDir);

		if (fDistance <= 5)
		{
			if (Engine::InputDev()->Key_Down(DIK_F))
			{
				if (!m_bQuestClearTalk)
				{
					if (Engine::UIManager()->Set_SpeechBubbleUse())
					{
						rPlayer.Set_Talk(true);

						if (!m_bTalking)
							m_bTalking = true;

						static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);
						CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
						CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_open.mp3", CHANNELID::SOUND_UI, 1.f);
					}
					m_bQuestClearTalk = true;
				}
				else if (m_iCount < m_iMaxCount)
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
					m_bTalking = false;
					rPlayer.Set_Talk(false);

					static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);

					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
					CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_close.mp3", CHANNELID::SOUND_UI, 1.f);

					m_bDead = true;
				}
			}
		}
	}

	if (m_bDead)
	{
		m_pTransform->m_vInfo[INFO_POS].y += fTimeDelta * 2.f;

		static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);

		if (m_pTransform->m_vInfo[INFO_POS].y >= 18.f)
		{
			rPlayer.Set_Quest(true);
			EventManager()->DeleteObject(this);

			Engine::UIManager()->Hide_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
			SceneManager()->Set_GameStop(false);
			m_bTalkingBox = false;
			m_bTalking = false;
			rPlayer.Set_Talk(false);

			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_UI);
			CSoundManager::GetInstance()->PlaySound(L"ui_dialogue_close.mp3", CHANNELID::SOUND_UI, 1.f);
		}
	}

	return iExit;
}

void CPhantom::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
}

void CPhantom::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
	if (!SceneManager()->Get_GameStop())
	{
		if ((!m_bTalkingBox) && m_bQuestClear && !m_bDead)
		{
			dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
			m_pFont->DrawText(L"F : TALK");
		}
	}
}

void CPhantom::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (OBJECTTAG::PLAYER == _pOther->Get_ObjectTag() && !m_bQuestClear)
	{
		_uint iPuzzleCount = dynamic_cast<CPlayer*>(_pOther->Get_Host())->Get_PuzzleResult();
		if (iPuzzleCount == 6)
			m_bQuestClear = true;


		if (m_bQuestClear)
		{
			m_bPush = false;
			// TODO
			// 퀘스트 클리어하고 접촉 시, 
		}
		else
		{
			CPlayer& rPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

			m_bPush = true;
			m_vInitPos = rPlayer.m_pTransform->m_vInfo[INFO_POS];
			rPlayer.Get_RigidBody()->Add_Force(_vec3(0.f, 14.f, 0.f));
		}
	}
}

void CPhantom::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CPhantom::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CPhantom::Add_Component()
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
	pComponent = m_pTexture[(_uint)STATE::IDLE] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_NpcPhantom"));
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

void CPhantom::Create_Puzzle()
{
	CGameObject* pGameObject = nullptr;

	pGameObject = CUIPuzzleBack::Create(m_pGraphicDev);
	NULL(pGameObject, E_FAIL);
    dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_PICTURE, 0);
    Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_MAP, Engine::UILAYER::UI_MIDDLE, pGameObject);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_MAP);

	pGameObject = CPuzzle_Part::Create(m_pGraphicDev, 0);
	NULL(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-83.f, 11.2f, -37.f));
	pGameObject->m_pTransform->Rotate(ROTATION::ROT_X, D3DXToRadian(90));
	EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

	pGameObject = CPuzzle_Part::Create(m_pGraphicDev, 1);
	NULL(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-89.f, 11.2f, -23.f));
	pGameObject->m_pTransform->Rotate(ROTATION::ROT_X, D3DXToRadian(90));
	EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);


	pGameObject = CPuzzle_Part::Create(m_pGraphicDev, 2);
	NULL(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-63.f, 11.2f, 5));
	pGameObject->m_pTransform->Rotate(ROTATION::ROT_X, D3DXToRadian(90));
	EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);


	pGameObject = CPuzzle_Part::Create(m_pGraphicDev, 3);
	NULL(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-83.f, 11.2f, 17));
	pGameObject->m_pTransform->Rotate(ROTATION::ROT_X, D3DXToRadian(90));
	EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);


	pGameObject = CPuzzle_Part::Create(m_pGraphicDev, 4);
	NULL(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-68, 11.2f, -27.f));
	pGameObject->m_pTransform->Rotate(ROTATION::ROT_X, D3DXToRadian(90));
	EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

	pGameObject = CPuzzle_Part::Create(m_pGraphicDev, 5);
	NULL(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-69.f, 11.2f, -43.f));
	pGameObject->m_pTransform->Rotate(ROTATION::ROT_X, D3DXToRadian(90));
	EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

	//pGameObject = CUIPuzzleBack::Create(m_pGraphicDev);
	//NULL(pGameObject, E_FAIL);
	//EventManager()->CreateObject(pGameObject, LAYERTAG::UI);
}

CPhantom* CPhantom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPhantom* pInstance = new CPhantom(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CPhantom*>(pInstance);

		MSG_BOX("Trader Npc Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPhantom::Free()
{
	__super::Free();
}
