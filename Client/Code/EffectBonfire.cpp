#include "stdafx.h"
#include "..\Header\EffectBonfire.h"
#include "PoolManager.h"

CEffectBonfire::CEffectBonfire(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectBonfire::~CEffectBonfire()
{
	Free();
}

HRESULT CEffectBonfire::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_BONEFIRE;

	m_pTransform->Translate(_vec3(0.f, 4.f, -22.f));

	m_fLife = 5.f;

	m_fEffectScale = .2f;

	return S_OK;
}

_int CEffectBonfire::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	m_fMoveTime += 1.f * fTimeDelta;

	if (m_fTime > m_fLife)
	{
		m_fTime = 0.f;
		m_fMoveTime = 0.f;

		m_fEffectScale = .2f;
		m_pTransform->m_vInfo[INFO_POS].x = 0.f;
		m_pTransform->m_vInfo[INFO_POS].y = 4.f;
		m_pTransform->m_vInfo[INFO_POS].z = -22.f;
		//CPoolManager::GetInstance()->Delete_Object(this);
	}

	m_pTransform->m_vInfo[INFO_POS].y += 2.f * fTimeDelta;
	if(m_fEffectScale > 0.f)
		m_fEffectScale -= .05f * fTimeDelta;

	if (m_fMoveTime < .5f)
	{
		m_pTransform->m_vInfo[INFO_POS].x += 2.f * fTimeDelta;
	}
	else if(.5f < m_fMoveTime)
	{
		m_pTransform->m_vInfo[INFO_POS].x -= 2.f * fTimeDelta;
		if (1.5f < m_fMoveTime)
		{
			m_fMoveTime = 0.f;
		}
	}

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectBonfire::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectBonfire::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectBonfire::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_FireEffect"));
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

CEffectBonfire* CEffectBonfire::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectBonfire* pInstance = new CEffectBonfire(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CEffectBonfire Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffectBonfire::Free()
{
	__super::Free();
}