#include "UISpeech_OldMan.h"
#include "Export_Function.h"
#include "Npc_OldMan.h"
#include "Player.h"
CUIspeech_OldMan::CUIspeech_OldMan(LPDIRECT3DDEVICE9 pGraphicDev)
	:CTempUI(pGraphicDev)
{
}

CUIspeech_OldMan::~CUIspeech_OldMan()
{
}

HRESULT CUIspeech_OldMan::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX/2;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY/4;

	m_pTransform->m_vLocalScale.x = 400.f;
	m_pTransform->m_vLocalScale.y = 100.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);


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

	m_iSpeech = 0;
	m_bQuest = false;
	return S_OK;
}

_int CUIspeech_OldMan::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;;
}

void CUIspeech_OldMan::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUIspeech_OldMan::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	srand(_uint(time(nullptr)));

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	m_pGameObject = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::NPC).front();

	vector<CGameObject*>& vecNpc =
		SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::NPC);

	NPCTAG eTargetTag = NPCTAG::OLD_MAN;
	CNpc* pTargetNpc = nullptr;

	auto FindNpcTag = [&eTargetTag](CGameObject* npc)
	{
		if (CNpc* npcCast = dynamic_cast<CNpc*>(npc))
			return npcCast->Get_NPCTag() == eTargetTag;

		return false;
	};

	auto Npciter = find_if(vecNpc.begin(), vecNpc.end(), FindNpcTag);

	if (Npciter != vecNpc.end())
		pTargetNpc = dynamic_cast<CNpc*>(*Npciter);

	if (pTargetNpc == nullptr)
		return;


	CInventory* PlayerInven = rPlayer.Get_Inventory();


	CItem* pItem = dynamic_cast<CItem*>(PlayerInven->Get_IDItem(ITEMID::QUEST_ORB));


	if (dynamic_cast<CNpc_OldMan*>(pTargetNpc)->IsTalk() && pItem == nullptr)
	{

		if (dynamic_cast<CNpc_OldMan*>(pTargetNpc)->Get_SpeechCount() == 0)
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"이렇게 다 죽어가는 마을에 새로운 손님은 오랜만이네");

			//dynamic_cast<CFont*>(m_pFont[1])->Set_pFont(m_pFontconfig[1]);
			//m_pFont[1]->DrawText(L"몬스터들을 무찌르고 오브를 찾아주신다니 정말 감사합니다.");
		}
		else if (1 == dynamic_cast<CNpc_OldMan*>(pTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[0]);
			m_pFont[0]->DrawText(L"낯선 젊은이,");

			dynamic_cast<CFont*>(m_pFont[1])->Set_pFont(m_pFontconfig[1]);
			m_pFont[1]->DrawText(L"이것도 인연인데 잠시 내 이야기 좀 들어주게나.");
		}
		else if (2 == dynamic_cast<CNpc_OldMan*>(pTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[0]);
			m_pFont[2]->DrawText(L"이 마을에는 옛날부터 대대로 내려오던 오브라는 것이 있었다네");

	/*		dynamic_cast<CFont*>(m_pFont[1])->Set_pFont(m_pFontconfig[1]);
			m_pFont[1]->DrawText(L"이것도 인연인데 잠시 내 이야기 좀 들어주게나.");*/
		}
		else if (3 == dynamic_cast<CNpc_OldMan*>(pTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[0]);
			m_pFont[0]->DrawText(L"오브는 이 뒤에 있는 마을을 지켜주던 큰 나무의 심장이자 원동력이었는데,");

			dynamic_cast<CFont*>(m_pFont[1])->Set_pFont(m_pFontconfig[1]);
			m_pFont[1]->DrawText(L"어느 날 몬스터들이 마을에 쳐들어와서 뺏어가고 말았다네..");
		}
		else if (4 == dynamic_cast<CNpc_OldMan*>(pTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[0]);
			m_pFont[0]->DrawText(L"그 뒤로 마을을 지켜주던 나무는 오브가 사라져 조금씩 죽어가고 있으며,");

			dynamic_cast<CFont*>(m_pFont[1])->Set_pFont(m_pFontconfig[1]);
			m_pFont[1]->DrawText(L"이 마을은 그로인해 생기를 잃어가며 새로운 손님들의 발길도 끊어지고 말았지..");
		}
		else if (5 == dynamic_cast<CNpc_OldMan*>(pTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[0]);
			m_pFont[2]->DrawText(L"이것도 인연인데, 몬스터를 무찌르고 오브를 찾아 우리 마을을 구해주게.");
		}
	}
	else if (dynamic_cast<CNpc_OldMan*>(pTargetNpc)->IsTalk() && pItem != nullptr)
	{
		if (dynamic_cast<CNpc_OldMan*>(pTargetNpc)->Get_SpeechCount() == 0)
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"그 많은 몬스터들을 무찌르고 오브를 찾아오다니 정말 대단하네.");
		}
		else if (1 == dynamic_cast<CNpc_OldMan*>(pTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[0]);
			m_pFont[0]->DrawText(L"내 뒤에 있는 제단에 오브를 올려놔주게,");

			dynamic_cast<CFont*>(m_pFont[1])->Set_pFont(m_pFontconfig[1]);
			m_pFont[1]->DrawText(L"그러면 이 마을의 생기는 다시 돌아올걸세");
		}
	}
}

HRESULT CUIspeech_OldMan::Add_Component(void)
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

void CUIspeech_OldMan::Key_Input(void)
{
}

CUIspeech_OldMan* CUIspeech_OldMan::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIspeech_OldMan* pInstance = new CUIspeech_OldMan(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UISoeech Bubble Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUIspeech_OldMan::Free()
{
	CTempUI::Free();
}
