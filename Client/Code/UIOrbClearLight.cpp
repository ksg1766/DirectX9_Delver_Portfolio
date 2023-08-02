#include "stdafx.h"
#include "..\Header\UIOrbClearLight.h"

CUIOrbClearLight::CUIOrbClearLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIOrbClearLight::~CUIOrbClearLight()
{

}

HRESULT CUIOrbClearLight::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // ÃÊ±âÈ­

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY / 2;

	m_pTransform->m_vLocalScale.x = 1000.f;
	m_pTransform->m_vLocalScale.y = 500.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	m_fFrame = 0.f;
	m_fFrist = 0.f;
	m_fFinal = 15.f;
	m_fFrameSpeed = 1.f;

	return S_OK;
}

_int CUIOrbClearLight::Update_Object(const _float & fTimeDelta)
{
	//if (m_IsDead)
		//return 0;

	if(m_bAnimation)
		m_fFrame += m_fFinal * fTimeDelta * m_fFrameSpeed;

	if (m_fFrame == m_fFinal) {
		m_fFrame = m_fFrist;
	}

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIOrbClearLight::LateUpdate_Object(void)
{
	//if (m_IsDead)
		//return;

	CTempUI::LateUpdate_Object();
}

void CUIOrbClearLight::Render_Object()
{
	//if (m_IsDead)
		//return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIOrbClearLight::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_UIOrbLight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIOrbClearLight::Key_Input(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	CGameObject* Obj = nullptr;

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y))
	{
		if (Engine::InputDev()->Mouse_Down(DIM_LB))
		{
		}
	}
}

CUIOrbClearLight* CUIOrbClearLight::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIOrbClearLight*	pInstance = new CUIOrbClearLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CUIOrbClearLight Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUIOrbClearLight::Free()
{
	CTempUI::Free();
}
