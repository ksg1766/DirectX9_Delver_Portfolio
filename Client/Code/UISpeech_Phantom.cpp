#include "UISpeech_Phantom.h"
#include "Export_Function.h"
#include "Phantom.h"
#include "Player.h"

CUISpeech_Phantom::CUISpeech_Phantom(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CUISpeech_Phantom::~CUISpeech_Phantom()
{
}

HRESULT CUISpeech_Phantom::Ready_Object()
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

_int CUISpeech_Phantom::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUISpeech_Phantom::LateUpdate_Object()
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUISpeech_Phantom::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	vector<CGameObject*>& vecNpc =
		SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::NPC);

	NPCTAG eTargetTag = NPCTAG::PAHNTOM;
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

	if (dynamic_cast<CPhantom*>(eTargetNpc)->IsTalk())
	{

		if (dynamic_cast<CPhantom*>(eTargetNpc)->Get_SpeechCount() == 0)
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"위험해!!");
		}
		else if (1 == dynamic_cast<CPhantom*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"이 안에는 엄청 큰 거미 몬스터가 있어.");
		}
		else if (2 == dynamic_cast<CPhantom*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"나도 너처럼 여기를 탐험하다가 안에 있는 거미 몬스터에게 죽고 말았어..");
		}
		else if (3 == dynamic_cast<CPhantom*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"집에서 내 소중한 가족들이 기다리고 있는데...");
		}
		else if (4 == dynamic_cast<CPhantom*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"거미 몬스터랑 싸우다가 내가 가지고 있던 가족사진이 다 찢어져 흩어지고 말았어");
		}
		else if (5 == dynamic_cast<CPhantom*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"이 안에 들어가기 전에 흩어진 내 가족사진을 찾아와줄래?");
		}
		else if (6 == dynamic_cast<CPhantom*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"정말 고마워, 마지막으로 사진으로라도 가족을 보고 싶었어.");
		}
		else if (7 == dynamic_cast<CPhantom*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"덕분에 여기서 성불할 수 있을 것 같아.");
		}
		else if (8 == dynamic_cast<CPhantom*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"친구는 나처럼 되지 말고 꼭 가족의 품으로 돌아가.");
		}
	}



}

HRESULT CUISpeech_Phantom::Add_Component()
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

void CUISpeech_Phantom::Key_Input()
{
}

CUISpeech_Phantom* CUISpeech_Phantom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUISpeech_Phantom* pInstance = new CUISpeech_Phantom(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CUISpeech_Phantom*>(pInstance);
		MSG_BOX("UITrander Create FAILED");
		return nullptr;
	}

	return pInstance;
}

void CUISpeech_Phantom::Free()
{
	__super::Free();
}
