#include "Npc_Bard.h"
#include "Export_Function.h"
#include "Bard_Idle.h"
#include "DynamicCamera.h"
#include "Player.h"

CNpc_Bard::CNpc_Bard(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CNpc(pGraphicDev)
{
}

CNpc_Bard::CNpc_Bard(const CNpc_Bard& rhs)
	: Engine::CNpc(rhs)
{
}

CNpc_Bard::~CNpc_Bard()
{
}

HRESULT CNpc_Bard::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::NPC;
	m_eNPCTag = NPCTAG::BARD;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Translate(_vec3(0.f, 3.f, 0.f));

	CState* pState = CNpc_Bard_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, pState);

	CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
		m_pTexture[(_uint)STATE::IDLE], STATE::IDLE, 8.f, TRUE);
	m_pAnimator->Add_Animation(STATE::IDLE, pAnimation);

	m_pStateMachine->Set_Animator(m_pAnimator);
	m_pStateMachine->Set_State(STATE::IDLE);

	m_pFontconfig = dynamic_cast<CFont*>(m_pFont)->Create_3DXFont(35, 28.f, 1000.f, false, L"Times New Roman", m_pFontconfig);
	dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
	dynamic_cast<CFont*>(m_pFont)->Set_FontColor(_uint(0xffffffff));
	dynamic_cast<CFont*>(m_pFont)->Set_Rect(RECT{ 0, 350, WINCX, 650 });
	dynamic_cast<CFont*>(m_pFont)->Set_Anchor(DT_CENTER | DT_NOCLIP);
	return S_OK;
}

_int CNpc_Bard::Update_Object(const _float& fTimeDelta)
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
			if (Engine::UIManager()->Set_SpeechBubbleUse())
			{
				m_bTalkingBox = true;

				rPlayer.Set_Talk(true);
				if (!m_bTalking)
					m_bTalking = true;

				static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			}
			else
			{
				Engine::UIManager()->Hide_PopupUI(UIPOPUPLAYER::POPUP_SPEECH);
				SceneManager()->Set_GameStop(false);
				m_bTalkingBox = false;
				m_bTalking = false;
				rPlayer.Set_Talk(false);

				static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
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

void CNpc_Bard::LateUpdate_Object()
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
}

void CNpc_Bard::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pStateMachine->Render_StateMachine();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
	if (!SceneManager()->Get_GameStop())
	{
		if ((!m_bTalkingBox) && (m_bTalkButton))
		{
			dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
			m_pFont->DrawText(L"F : TALK");
		}
	}
}

void CNpc_Bard::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

}

void CNpc_Bard::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CNpc_Bard::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CNpc_Bard::Add_Component()
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
	pComponent = m_pTexture[(_uint)STATE::IDLE] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_NpcBard"));
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

CNpc_Bard* CNpc_Bard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_Bard* pInstance = new CNpc_Bard(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CNpc_Bard*>(pInstance);

		MSG_BOX("Trader Npc Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNpc_Bard::Free()
{
	__super::Free();
}
