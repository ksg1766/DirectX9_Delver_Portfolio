#include "stdafx.h"
#include "Export_Function.h"
#include "..\Header\UIPuzzleBack.h"
#include "UIPuzzle.h"

CUIPuzzleBack::CUIPuzzleBack(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CUIPuzzleBack::~CUIPuzzleBack()
{
}

HRESULT CUIPuzzleBack::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL);
	FAILED_CHECK(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX * 0.4;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY * 0.5;
	m_pTransform->m_vLocalScale.x = 320.f;
	m_pTransform->m_vLocalScale.y = 250.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y,
		m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);


	return S_OK;
}

_int CUIPuzzleBack::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIPuzzleBack::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUIPuzzleBack::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();


	for (auto iter : m_vecPuzzle)
		iter->Render_Object();
}

HRESULT CUIPuzzleBack::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_QuestItemBackGround"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIPuzzleBack::Key_Input(void)
{
}

CUIPuzzleBack* CUIPuzzleBack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIPuzzleBack* pInstance = new CUIPuzzleBack(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CUIPuzzleBack*>(pInstance);
		MSG_BOX("PuzzleBack Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUIPuzzleBack::Free()
{
	CTempUI::Free();
}
