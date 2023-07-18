#include "stdafx.h"
#include "..\Header\EffectTwinkle.h"

CEffectTwinkle::CEffectTwinkle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectTwinkle::~CEffectTwinkle()
{
	Free();
}

HRESULT CEffectTwinkle::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_TWINKLE;

	m_bAnimation = true;

	m_fFrame = 0.f;
	m_fFrist = 0.f;
	m_fFinal = 5.f;
	m_fFrameSpeed = CTempEffect::Get_RandomFloat(1.f, 2.f);

	m_fLife       = CTempEffect::Get_RandomFloat(5.f, 10.f);

	m_fEffectScale = CTempEffect::Get_RandomFloat(1.f, 1.5f);

	return S_OK;
}

Engine::_int CEffectTwinkle::Update_Object(const _float& fTimeDelta)
{
	if (m_RandomSet)
	{
		m_RandomSet = false;

	}

	// 타겟 따라다니면서 뒤쪽에 반짝이들 생성


	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectTwinkle::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectTwinkle::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectTwinkle::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectDebuff"));
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

void CEffectTwinkle::Free()
{
	CTempEffect::Free();
}

CEffectTwinkle* CEffectTwinkle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectTwinkle*	pInstance = new CEffectTwinkle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectTwinkle Create Failed");
		return nullptr;
	}

	return pInstance;
}
