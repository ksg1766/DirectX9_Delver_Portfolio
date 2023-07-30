#include "stdafx.h"
#include "..\Header\EffectWaterdrop.h"
#include "PoolManager.h"

CEffectWaterdrop::CEffectWaterdrop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectWaterdrop::~CEffectWaterdrop()
{
	Free();
}

HRESULT CEffectWaterdrop::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_BLOOD;

	m_fLife  = 10.f;
	m_pTransform->Rotate(ROT_X, D3DXToRadian(90.f));
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(CTempEffect::Get_RandomFloat(.0f, 180.f)));

	m_bParent = true;

	return S_OK;
}

Engine::_int CEffectWaterdrop::Update_Object(const _float& fTimeDelta)
{
	if (m_RandomSet)
	{
		m_RandomSet = false;
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectWaterdrop::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();
}

void CEffectWaterdrop::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectWaterdrop::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectBlood"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CEffectWaterdrop::Free()
{
	CTempEffect::Free();
}

CEffectWaterdrop* CEffectWaterdrop::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectWaterdrop*	pInstance = new CEffectWaterdrop(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectWaterdrop Create Failed");
		return nullptr;
	}

	return pInstance;
}
