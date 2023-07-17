#include "stdafx.h"
#include "..\Header\EffectTrace.h"

CEffectTrace::CEffectTrace(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectTrace::~CEffectTrace()
{
	Free();
}

HRESULT CEffectTrace::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fFrame = 3.f;
	m_fLife  = 3.f;

	m_fEffectScale = .15f;

	return S_OK;
}

Engine::_int CEffectTrace::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (m_RandomSet)
	{
		m_RandomSet = false;
		switch (m_EffectColor)
		{
		case Engine::ECOLOR_PINK:
			m_fFrame = 0.f;
			break;
		case Engine::ECOLOR_RED:
			m_fFrame = 1.f;
			break;
		case Engine::ECOLOR_ORANGE:
			m_fFrame = 2.f;
			break;
		case Engine::ECOLOR_YELLOW:
			m_fFrame = 3.f;
			break;
		case Engine::ECOLOR_LIGHTGREEN:
			m_fFrame = 4.f;
			break;
		case Engine::ECOLOR_GREEN:
			m_fFrame = 5.f;
			break;
		case Engine::ECOLOR_SKYBLUE:
			m_fFrame = 6.f;
			break;
		case Engine::ECOLOR_BLUE:
			m_fFrame = 7.f;
			break;
		case Engine::ECOLOR_INDIGO:
			m_fFrame = 8.f;
			break;
		case Engine::ECOLOR_PURPLE:
			m_fFrame = 9.f;
			break;
		case Engine::ECOLOR_WHITE:
			m_fFrame = 10.f;
			break;
		case Engine::ECOLOR_RAINBOW:
			m_fFrame = 10.f;
			break;
		default:
			m_fFrame = 10.f;
			break;
		}
	}

	if (m_fEffectScale > 0.f) {
		m_fEffectScale -= 0.02f * fTimeDelta * (m_fTraceNum / 5);
	}

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectTrace::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectTrace::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectTrace::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectTrace"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBillBoardCom = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CEffectTrace::Free()
{
	CTempEffect::Free();
}

CEffectTrace* CEffectTrace::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectTrace*	pInstance = new CEffectTrace(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectTrace Create Failed");
		return nullptr;
	}

	return pInstance;
}
