#include "stdafx.h"
#include "UITitle.h"
#include "Export_Function.h"


CUITitle::CUITitle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUITitle::~CUITitle()
{

}

HRESULT CUITitle::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // 초기화

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX * 0.5f;
	m_pTransform->m_vInfo[INFO_POS].y = WINCY * 0.8f;
	m_pTransform->m_vLocalScale.x = 320.f;
	m_pTransform->m_vLocalScale.y = 250.f;

	WorldMatrix
	(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y,
		m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

	m_fAlpha = 255.f;

	return S_OK;
}

_int CUITitle::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUITitle::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUITitle::Render_Object()
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(7);
	m_pBufferCom->Render_Buffer();

	if (m_bFirst)
	{
		const float targetAlpha = 0.f;
		const float alphaSpeed = 80.f;

		m_fAlpha = max(m_fAlpha - alphaSpeed * Engine::Get_TimeDelta(L"Timer_FPS60"), targetAlpha);
		
		if (m_fAlpha <= 15)
		{
			m_bFirst = false;
			m_bHide = true;
			m_fAlpha = 220.f;
		}
	}
	else if (m_bHide)
	{
		const _float fTargetAlpha = 255.f;
		const _float fAlphaSpped = 80.f;

		m_fAlpha = min(m_fAlpha + fAlphaSpped * Engine::Get_TimeDelta(L"Timer_FPS60"), fTargetAlpha);

		if (m_fAlpha >= 250.f)
		{
			Engine::UIManager()->Delete_BasicObject(UILAYER::UI_UP);
			m_IsDead = true;
		}


	}

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, (_uint)(m_fAlpha));
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	// 글자의 알파값을 적용하여 렌더링
	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();

	// 알파 테스트 설정 해제
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0); // 기본값으로 리셋
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS); // 기본값으로 리셋
}

HRESULT CUITitle::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_UITitle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CUITitle* CUITitle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUITitle* pInstance = new CUITitle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUITitle::Free()
{
	CTempUI::Free();
}
