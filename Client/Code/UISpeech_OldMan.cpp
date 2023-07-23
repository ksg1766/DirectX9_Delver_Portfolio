#include "UISpeech_OldMan.h"
#include "Export_Function.h"
#include "Npc_OldMan.h"
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


	m_pFontconfig[0] = dynamic_cast<CFont*>(m_pFont)->Create_3DXFont(32, 13.f, 1000.f, false, TEXT("¸¼Àº °íµñ"), m_pFontconfig[0]);
	dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig[0]);
	dynamic_cast<CFont*>(m_pFont)->Set_FontColor(_uint(0xffffffff));
	dynamic_cast<CFont*>(m_pFont)->Set_Rect(RECT{ 0, 520, WINCX, WINCY });
	dynamic_cast<CFont*>(m_pFont)->Set_Anchor(DT_CENTER | DT_NOCLIP);

	m_pFontconfig[1] = dynamic_cast<CFont*>(m_pFont)->Create_3DXFont(32, 15.f, 1000.f, false, TEXT("¸¼Àº °íµñ"), m_pFontconfig[1]);
	dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig[1]);
	dynamic_cast<CFont*>(m_pFont)->Set_FontColor(_uint(0xffffffff));
	dynamic_cast<CFont*>(m_pFont)->Set_Rect(RECT{ 0, 520, WINCX, WINCY });
	dynamic_cast<CFont*>(m_pFont)->Set_Anchor(DT_CENTER | DT_NOCLIP);

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

	/*dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig[1]);
	m_pFont->DrawText(L"");*/
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


	if (dynamic_cast<CNpc_OldMan*>(pTargetNpc)->IsTalk())
	{
		if (!dynamic_cast<CNpc_OldMan*>(m_pGameObject)->Get_Quest())
		{
			dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig[1]);
			m_pFont->DrawText(L"³ª¹«Á» °íÃÄÁÖ°ÔÀ×");
		}
		else
		{
			switch (dynamic_cast<CNpc_OldMan*>(m_pGameObject)->Get_Speech())
			{
			case 0:
				dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig[1]);
				m_pFont->DrawText(L"»¡¸® °¡¼­ ÇØÁàÀ×");
				break;
			case 1:	dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig[1]);
				m_pFont->DrawText(L"¿©±â¼­ ¹» ÇÏ°í ÀÖ³ª");
				break;
			case 2:
				dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig[1]);
				m_pFont->DrawText(L"¿¡ÀÕ »¡¸® ²¨Á®À×");
				break;
			}
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

	pComponent = m_pFont = dynamic_cast<CFont*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Font"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::FONT, pComponent);

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
