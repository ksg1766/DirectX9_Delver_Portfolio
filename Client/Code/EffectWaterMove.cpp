#include "stdafx.h"
#include "..\Header\EffectWaterMove.h"
#include "PoolManager.h"

CEffectWaterMove::CEffectWaterMove(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectWaterMove::~CEffectWaterMove()
{
	Free();
}

HRESULT CEffectWaterMove::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_BLOOD;

	m_fEffectScale = 1.f;
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));

	return S_OK;
}

Engine::_int CEffectWaterMove::Update_Object(const _float& fTimeDelta)
{
	if (m_RandomSet) {
		m_RandomSet = false;
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectWaterMove::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();
}

void CEffectWaterMove::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectWaterMove::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectWaterMove"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CEffectWaterMove::Free()
{
	CTempEffect::Free();
}

CEffectWaterMove* CEffectWaterMove::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectWaterMove*	pInstance = new CEffectWaterMove(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectWaterMove Create Failed");
		return nullptr;
	}

	return pInstance;
}
