#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CUIManager)

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
	Free();
}

void CUIManager::Add_UIGameobject(UILAYER eType, CGameObject* pGameObject)
{
	if (UILAYER::UI_END <= eType || nullptr == pGameObject)
		return;

	m_UIList[eType].push_back(pGameObject);
	//pGameObject->AddRef();
}

_int CUIManager::Update_UI(const _float& fTimeDelta)
{
	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto iter : m_UIList[i])
			iter->Update_Object(fTimeDelta);
	}

	return _int();
}

void CUIManager::LateUpdate_UI()
{
	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto iter : m_UIList[i])
			iter->LateUpdate_Object();
	}
}

void CUIManager::Render_UI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	D3DVIEWPORT9 m_BackupViewPort;
	pGraphicDev->GetViewport(&m_BackupViewPort); // 현재 뷰 포트 백업

	// UI용 새로운 뷰 포트 생성 및 적용
	D3DVIEWPORT9 UiViewPort;
	UiViewPort.X = 0;
	UiViewPort.Y = 0;
	UiViewPort.Width = WINCX;
	UiViewPort.Height = WINCY;
	UiViewPort.MinZ = 0;
	UiViewPort.MaxZ = 1;
	pGraphicDev->SetViewport(&UiViewPort);

	_matrix matView;
	pGraphicDev->SetTransform(D3DTS_VIEW, D3DXMatrixIdentity(&matView)); // 항등행렬로 적용된 뷰 행렬 초기화.

	_matrix	m_matProj;
	D3DXMatrixOrthoOffCenterLH(&m_matProj, 0, WINCX, 0, WINCY, -1.f, 1.f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);   // 직교투영 행렬 적용.

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);    // Z버퍼 OFF

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파렌더링 ON
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto iter : m_UIList[i])
			iter->Render_Object();
	}

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // 알파렌더링 OFF

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      // Z버퍼 ON


	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);    // UI 전체 출력 후 다시 원근투영 행렬 적용.


	pGraphicDev->SetViewport(&m_BackupViewPort);                // UI 전체 출력 후 백업해둔 이전 뷰포트로 되돌림.
}

void CUIManager::Free()
{
	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for_each(m_UIList[i].begin(), m_UIList[i].end(), CDeleteObj());
		m_UIList[i].clear();
	}
}