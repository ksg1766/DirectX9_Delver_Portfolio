#include "stdafx.h"
#include "..\Header\UImouse.h"

CUImouse::CUImouse(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUImouse::~CUImouse()
{

}

HRESULT CUImouse::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = 400.f;
	m_pTransform->m_vInfo[INFO_POS].y = 300.f;
	m_pTransform->m_vLocalScale.x = 40.f;
	m_pTransform->m_vLocalScale.y = 40.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);



	return S_OK;
}

_int CUImouse::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CTempUI::Update_Object(fTimeDelta);

	//POINT pt{};
	//GetCursorPos(&pt);
	//ClientToScreen(g_hWnd, &pt);

	HCURSOR cursor = GetCursor(); // 현재 마우스 커서를 가져옴

// 마우스 커서를 숨기는 옵션으로 변경
	cursor = LoadCursor(NULL, IDC_HAND); // IDC_ARROW 대신에 다른 커서 스타일을 지정할 수 있습니다.

	SetCursor(cursor); // 변경된 마우스 커서를 설정

	return iExit;
}

void CUImouse::LateUpdate_Object(void)
{
	CTempUI::LateUpdate_Object();
}

void CUImouse::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUImouse::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTexBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_mouseUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUImouse::Key_Input(void)
{
}

CUImouse* CUImouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUImouse*	pInstance = new CUImouse(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUImouse::Free()
{
	CTempUI::Free();
}
