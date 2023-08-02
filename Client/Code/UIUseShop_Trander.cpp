#include "..\Header\UIUseShop_Trander.h"
#include "Export_Function.h"
#include "Npc_Trader.h"
#include "Player.h"

CUIUseShop_Trander::CUIUseShop_Trander(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CUIUseShop_Trander::~CUIUseShop_Trander()
{
}

HRESULT CUIUseShop_Trander::Ready_Object()
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

_int CUIUseShop_Trander::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIUseShop_Trander::LateUpdate_Object()
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUIUseShop_Trander::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	vector<CGameObject*>& vecNpc = 
		SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::NPC);

	NPCTAG eTargetTag = NPCTAG::TRADER;
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

	CInventory* PlayerInven = rPlayer.Get_Inventory();


	CItem* pItem = dynamic_cast<CItem*>(PlayerInven->Get_IDItem(ITEMID::QUEST_ORB));



	if (dynamic_cast<CNpc_Trader*>(eTargetNpc)->IsTalk() && pItem == nullptr)
	{

		if (dynamic_cast<CNpc_Trader*>(eTargetNpc)->Get_SpeechCount() == 0)
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[0]);
			m_pFont[0]->DrawText(L"촌장님한테 이야기 들었습니다.");

			dynamic_cast<CFont*>(m_pFont[1])->Set_pFont(m_pFontconfig[1]);
			m_pFont[1]->DrawText(L"몬스터들을 무찌르고 오브를 찾아주신다니 정말 감사합니다.");
		}
		else if (1 == dynamic_cast<CNpc_Trader*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[0]);
			m_pFont[0]->DrawText(L"제가 같이 갈 수는 없으니");

			dynamic_cast<CFont*>(m_pFont[1])->Set_pFont(m_pFontconfig[1]);
			m_pFont[1]->DrawText(L"제가 가지고 있는 다양한 아이템들이라도 지원해 드리겠습니다.");
		}
		else if (2 == dynamic_cast<CNpc_Trader*>(eTargetNpc)->Get_SpeechCount())
		{
			dynamic_cast<CFont*>(m_pFont[0])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"부디 오브를 찾아 이 마을을 꼭 구해주세요");
		}
	}
	else if (dynamic_cast<CNpc_Trader*>(eTargetNpc)->IsTalk() && pItem != nullptr)
	{
		if (dynamic_cast<CNpc_Trader*>(eTargetNpc)->Get_SpeechCount() == 0)
		{
			dynamic_cast<CFont*>(m_pFont[2])->Set_pFont(m_pFontconfig[2]);
			m_pFont[2]->DrawText(L"오브를 찾아와주셨다면서요!! 정말 감사합니다.");
		}
	}

}

HRESULT CUIUseShop_Trander::Add_Component()
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

void CUIUseShop_Trander::Key_Input()
{
}

CUIUseShop_Trander* CUIUseShop_Trander::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIUseShop_Trander* pInstance = new CUIUseShop_Trander(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CUIUseShop_Trander*>(pInstance);
		MSG_BOX("UITrander Create FAILED");
		return nullptr;
	}

	return pInstance;
}

void CUIUseShop_Trander::Free()
{
	__super::Free();
}
