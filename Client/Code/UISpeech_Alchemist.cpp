#include "UISpeech_Alchemist.h"
#include "Export_Function.h"
#include "Npc_Alchemist.h"

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

	m_pFontconfig = dynamic_cast<CFont*>(m_pFont)->Create_3DXFont(32, 13.f, 1000.f, false, TEXT("���� ����"), m_pFontconfig);
	dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
	dynamic_cast<CFont*>(m_pFont)->Set_FontColor(_uint(0xffffffff));
	dynamic_cast<CFont*>(m_pFont)->Set_Rect(RECT{ 0, 520, WINCX, WINCY });
	dynamic_cast<CFont*>(m_pFont)->Set_Anchor(DT_CENTER | DT_NOCLIP);

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

	if (dynamic_cast<CNpc_Alchemist*>(eTargetNpc)->IsTalk())
	{
		dynamic_cast<CFont*>(m_pFont)->Set_pFont(m_pFontconfig);
		m_pFont->DrawText(L"�ϴ� ����� ����");
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

	pComponent = m_pFont = dynamic_cast<CFont*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Font"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::FONT, pComponent);

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