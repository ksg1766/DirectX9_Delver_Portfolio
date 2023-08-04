#include "stdafx.h"
#include "Export_Function.h"
#include "UIPuzzle.h"

CUIPuzzle::CUIPuzzle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CUIPuzzle::~CUIPuzzle()
{
}

HRESULT CUIPuzzle::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL);
	FAILED_CHECK(Add_Component(), E_FAIL);



	return S_OK;
}

_int CUIPuzzle::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIPuzzle::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUIPuzzle::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	if (m_fCurrentImage == 0)
	{
		m_pTransform->m_vInfo[INFO_POS].x = WINCX * 0.4;
		m_pTransform->m_vInfo[INFO_POS].y = WINCY * 0.5;
		m_pTransform->m_vLocalScale.x = 320.f;
		m_pTransform->m_vLocalScale.y = 250.f;


		WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y,
			m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	}
	else if (m_fCurrentImage == 1)
	{
		m_pTransform->m_vInfo[INFO_POS].x = WINCX * 0.4;
		m_pTransform->m_vInfo[INFO_POS].y = WINCY * 0.5;
		m_pTransform->m_vLocalScale.x = 320.f;
		m_pTransform->m_vLocalScale.y = 250.f;


		WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y,
			m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	}
	else if (m_fCurrentImage == 2)
	{
		m_pTransform->m_vInfo[INFO_POS].x = WINCX * 0.4;
		m_pTransform->m_vInfo[INFO_POS].y = WINCY * 0.5;
		m_pTransform->m_vLocalScale.x = 320.f;
		m_pTransform->m_vLocalScale.y = 250.f;


		WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y,
			m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	}
	else if (m_fCurrentImage == 3)
	{
		m_pTransform->m_vInfo[INFO_POS].x = WINCX * 0.4;
		m_pTransform->m_vInfo[INFO_POS].y = WINCY * 0.5;
		m_pTransform->m_vLocalScale.x = 320.f;
		m_pTransform->m_vLocalScale.y = 250.f;


		WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y,
			m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	}
	else if (m_fCurrentImage == 4)
	{
		m_pTransform->m_vInfo[INFO_POS].x = WINCX * 0.4;
		m_pTransform->m_vInfo[INFO_POS].y = WINCY * 0.5;
		m_pTransform->m_vLocalScale.x = 320.f;
		m_pTransform->m_vLocalScale.y = 250.f;


		WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y,
			m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	}
	else if (m_fCurrentImage == 5)
	{
		m_pTransform->m_vInfo[INFO_POS].x = WINCX * 0.4;
		m_pTransform->m_vInfo[INFO_POS].y = WINCY * 0.5;
		m_pTransform->m_vLocalScale.x = 320.f;
		m_pTransform->m_vLocalScale.y = 250.f;


		WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y,
			m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	}

	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIPuzzle::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_UIQuestItem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIPuzzle::Key_Input(void)
{
}

CUIPuzzle* CUIPuzzle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIPuzzle* pInstance = new CUIPuzzle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CUIPuzzle*>(pInstance);
		MSG_BOX("PuzzleBack Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUIPuzzle::Free()
{
	CTempUI::Free();
}
