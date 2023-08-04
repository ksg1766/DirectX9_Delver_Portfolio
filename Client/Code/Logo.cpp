#include "stdafx.h"
#include "..\Header\Logo.h"
#include "SoundManager.h"
#include "Export_Function.h"
//#include "Stage.h"
//#include "Editor.h"
#include "BlackIn.h"
#include "BlackOutIn.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	m_eSceneTag = SCENETAG::LOGO;
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	
	//m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADINGID::LOADING_STAGE);
	//m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADINGID::LOADING_EDITOR);
	//NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	HCURSOR Cursor = GetCursor();
	Cursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(Cursor);

	CSoundManager::GetInstance()->PlayBGM(L"title.mp3", 1.f);

	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit =__super::Update_Scene(fTimeDelta);

	UIManager()->Update_UI(fTimeDelta);

	if (!m_bClick && Engine::InputDev()->Get_AnyKeyDown())
	{
		m_bClick = true;

		Engine::CGameObject* pGameObject = CBlackOutIn::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		static_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_BASIC, 1);
		Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_UP, pGameObject);

		// 로고랑 프래스 폰트 삭제
		Engine::UIManager()->Delete_BasicObject(Engine::UILAYER::UI_MIDDLE);
	}

	if (m_bClick)
	{
		CGameObject* pBlackOut = Engine::UIManager()->Get_BasicObject(UI_UP, UIID_BASIC, 1);
		if (pBlackOut != nullptr && static_cast<CBlackOutIn*>(pBlackOut)->Get_Middle() && !m_bFadeEnd)
		{
			m_bFadeEnd = true;

			// 버튼 3개 + 글씨 생성
			Engine::CGameObject* pGameObject = CStartButton::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_MIDDLE, pGameObject);

			pGameObject = CEditButton::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_MIDDLE, pGameObject);

			pGameObject = CExitButton::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_MIDDLE, pGameObject);

			pGameObject = CSelectFont::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_MIDDLE, pGameObject);
		}
	}

	return iExit;
}

void CLogo::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();

	UIManager()->LateUpdate_UI();
}

void CLogo::Render_Scene()
{
	// DEBUG 용 렌더
}

void CLogo::Free()
{
	//Safe_Release(m_pLoading);
	CSoundManager::GetInstance()->StopAll();
	__super::Free();
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
 	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CLogo::Ready_Prototype()
{
	// 로고 씬에서 사용될 컴포넌트
	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Transform_Logo", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Logo", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Startscreen/Startscreen%d.png",11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_FadeIn", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/BlackIn/black%d.png", 20)), E_FAIL);
    FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_FadeOut", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/BlackOutIn/black%d.png", 38)), E_FAIL);
	FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/title.mp3", "title.mp3"), E_FAIL);
	
	return S_OK;
}

HRESULT CLogo::Ready_Layer_Environment(LAYERTAG _eLayerTag)
{
	//Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	//NULL_CHECK_RETURN(pLayer, E_FAIL);
	//m_mapLayer.insert({ _eLayerTag, pLayer });

	Engine::CGameObject*		pGameObject = nullptr;
	
	// BackGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_DOWN, pGameObject);
	//Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_STAT, Engine::UILAYER::UI_DOWN, pGameObject);

	pGameObject = CGameLogo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_MIDDLE, pGameObject);
	
	pGameObject = CPressFont::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_MIDDLE, pGameObject);

	pGameObject = CBlackIn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	static_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_BASIC, 0);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_BLACK, Engine::UILAYER::UI_DOWN, pGameObject);
	//Engine::EventManager()->CreateObject(pGameObject, _eLayerTag);

	return S_OK;
}
