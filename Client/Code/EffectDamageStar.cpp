#include "stdafx.h"
#include "..\Header\EffectDamageStar.h"

CEffectDamageStar::CEffectDamageStar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectDamageStar::~CEffectDamageStar()
{
	Free();
}

HRESULT CEffectDamageStar::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_DAMAGESTAR;

	m_bAnimation = true;
	m_bLoop      = true;

	m_fFrame = 0.f;
	m_fFrist = 0.f;
	m_fFinal = 8.f;
	m_fFrameSpeed = 2.f;

	m_fLife       = 1.5f;

	m_fEffectScale = 1.f;

	return S_OK;
}

Engine::_int CEffectDamageStar::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	if (m_pTarget != nullptr)
	{
		m_pTransform->m_vInfo[INFO_POS].x = m_pTarget->m_pTransform->m_vInfo[INFO_POS].x;
		m_pTransform->m_vInfo[INFO_POS].y = m_pTarget->m_pTransform->m_vInfo[INFO_POS].y + 0.7f;
		m_pTransform->m_vInfo[INFO_POS].z = m_pTarget->m_pTransform->m_vInfo[INFO_POS].z;
	}

	return iExit;
}

void CEffectDamageStar::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectDamageStar::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectDamageStar::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectDamageStar"));
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

void CEffectDamageStar::Free()
{
	CTempEffect::Free();
}

CEffectDamageStar* CEffectDamageStar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectDamageStar*	pInstance = new CEffectDamageStar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectDamageStar Create Failed");
		return nullptr;
	}

	return pInstance;
}
