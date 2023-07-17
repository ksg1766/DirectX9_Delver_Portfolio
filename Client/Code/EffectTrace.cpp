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
