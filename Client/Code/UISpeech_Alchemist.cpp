#include "UISpeech_Alchemist.h"
#include "Export_Function.h"
#include "Npc_Alchemist.h"
#include "Player.h"

CUISpeech_Alchemist::CUISpeech_Alchemist(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CUISpeech_Alchemist::~CUISpeech_Alchemist()
{
}

HRESULT CUISpeech_Alchemist::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX * 0.5;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY * 0.25;

	m_pTransform->m_vLocalScale.x = 400.f;
	m_pTransform->m_vLocalScale.y = 100.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y,
		m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);


	m_pFontconfig[0] = dynamic_cast<CFont*>(m_pFont[0])->Create_3DXFont(32, 15.f, 1000.f, false, TEXT("둥근모꼴"), m_pFontconfig[0]);
	dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[0]);
	dynamic_cast<CFont*>(m_pFont[0])->Set_FontColor(_uint(0xffffffff));
	dynamic_cast<CFont*>(m_pFont[0])->Set_Rect(RECT{ 0, 490, WINCX, 540 });
	dynamic_cast<CFont*>(m_pFont[0])->Set_Anchor(DT_CENTER | DT_NOCLIP);

	m_pFontconfig[1] = dynamic_cast<CFont*>(m_pFont[1])->Create_3DXFont(32, 15.f, 1000.f, false, TEXT("둥근모꼴"), m_pFontconfig[1]);
	dynamic_cast<CFont*>(m_pFont[1])->Set_pFont(m_pFontconfig[1]);
	dynamic_cast<CFont*>(m_pFont[1])->Set_FontColor(_uint(0xffffffff));
	dynamic_cast<CFont*>(m_pFont[1])->Set_Rect(RECT{ 0, 545, WINCX, 600 });
	dynamic_cast<CFont*>(m_pFont[1])->Set_Anchor(DT_CENTER | DT_NOCLIP);

	m_pFontconfig[2] = dynamic_cast<CFont*>(m_pFont[2])->Create_3DXFont(32, 15.f, 1000.f, false, TEXT("둥근모꼴"), m_pFontconfig[2]);
	dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
	dynamic_cast<CFont*>(m_pFont[2])->Set_FontColor(_uint(0xffffffff));
	dynamic_cast<CFont*>(m_pFont[2])->Set_Rect(RECT{ 0, 520, WINCX, WINCY });
	dynamic_cast<CFont*>(m_pFont[2])->Set_Anchor(DT_CENTER | DT_NOCLIP);

	return S_OK;
}

_int CUISpeech_Alchemist::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUISpeech_Alchemist::LateUpdate_Object()
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUISpeech_Alchemist::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	vector<CGameObject*>& vecNpc =
		SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::NPC);

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	NPCTAG eTargetTag = NPCTAG::ALCHEMIST;
	CNpc* eTargetNpc = nullptr;

	auto FindNpcTag = [&eTargetTag](CGameObject* npc)
	{
		if (CNpc* npcCast = dynamic_cast<CNpc*>(npc))
			return npcCast->Get_NPCTag() == eTargetTag;

		return false;
	};

	auto Npciter = find_if(vecNpc.begin(), vecNpc.end(), FindNpcTag);

	if (Npciter != vecNpc.end())
		eTargetNpc = dynamic_cast<CNpc*>(*Npciter);

	if (eTargetNpc == nullptr)
		return;

	if (dynamic_cast<CNpc_Alchemist*>(eTargetNpc)->IsTalk() && !rPlayer.AltarOnOrb())
	{

		if (dynamic_cast<CNpc_Alchemist*>(eTargetNpc)->Get_SpeechCount() == 0)
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"왕년에는 내가 마법으로 전 세계를 날아다녔는데..");
		}
		else if (1 == dynamic_cast<CNpc_Alchemist*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[0]);
			m_pFont[0]->DrawText(L"몬스터들에게 오브를 빼앗기고,");

			dynamic_cast<CFont*>(m_pFont[1])->Set_pFont(m_pFontconfig[1]);
			m_pFont[1]->DrawText(L"마을을 지켜주던 나무가 죽어가니 잘 되던 마법이 안되기 시작했어..");
		}
		else if (2 == dynamic_cast<CNpc_Alchemist*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"...이제 마법을 못 쓰면 뭐하고 살아야 할 지 모르겠어");
		}
		else if (3 == dynamic_cast<CNpc_Alchemist*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L".....");
		}
	}
	else if (dynamic_cast<CNpc_Alchemist*>(eTargetNpc)->IsTalk() && rPlayer.AltarOnOrb())
	{
		if (dynamic_cast<CNpc_Alchemist*>(eTargetNpc)->Get_SpeechCount() == 0)
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"마법이 다시 나오기 시작했어...!");
		}
		else if (1 == dynamic_cast<CNpc_Alchemist*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"정말 다행이야...");
		}
	}
}

HRESULT CUISpeech_Alchemist::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SpeechBubbleUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (_uint i = 0; i < 3; ++i)
	{
		pComponent = m_pFont[i] = dynamic_cast<CFont*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Font"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::FONT, pComponent);
	}

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUISpeech_Alchemist::Key_Input()
{
}

CUISpeech_Alchemist* CUISpeech_Alchemist::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUISpeech_Alchemist* pInstance = new CUISpeech_Alchemist(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CUISpeech_Alchemist*>(pInstance);
		MSG_BOX("UITrander Create FAILED");
		return nullptr;
	}

	return pInstance;
}

void CUISpeech_Alchemist::Free()
{
	__super::Free();
}
