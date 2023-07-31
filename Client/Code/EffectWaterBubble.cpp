#include "stdafx.h"
#include "..\Header\EffectWaterBubble.h"
#include "PoolManager.h"

CEffectWaterBubble::CEffectWaterBubble(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectWaterBubble::~CEffectWaterBubble()
{
	Free();
}

HRESULT CEffectWaterBubble::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_WATERFALL;

	return S_OK;
}

Engine::_int CEffectWaterBubble::Update_Object(const _float& fTimeDelta)
{
	if (m_RandomSet)
	{
		m_RandomSet = false;
		m_fFrame = CTempEffect::Get_RandomFloat(0.f, 3.f);
		m_fEffectScale = CTempEffect::Get_RandomFloat(m_vecBubbleScale[0].x, m_vecBubbleScale[1].x);
		m_fSpeed = CTempEffect::Get_RandomFloat(1.f, 5.f);
		m_pTransform->m_vInfo[INFO_POS].x = CTempEffect::Get_RandomFloat(m_vecOriginPos.x + m_EffectBoundingBox.vMin.x, m_vecOriginPos.x + m_EffectBoundingBox.vMax.x);
		m_pTransform->m_vInfo[INFO_POS].z = CTempEffect::Get_RandomFloat(m_vecOriginPos.z + m_EffectBoundingBox.vMin.z, m_vecOriginPos.z + m_EffectBoundingBox.vMax.z);
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	m_pTransform->m_vInfo[INFO_POS].y += 1.f * fTimeDelta * m_fSpeed;

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectWaterBubble::LateUpdate_Object(void)
{
	if (m_pTransform->m_vInfo[INFO_POS].y > m_vecOriginPos.y + 10.f)
	{
		// 재활용
		m_pTransform->m_vInfo[INFO_POS].y = m_vecOriginPos.y;
		m_fFrame = CTempEffect::Get_RandomFloat(0.f, 3.f);
		m_fEffectScale = CTempEffect::Get_RandomFloat(m_vecBubbleScale[0].x, m_vecBubbleScale[1].x);
		m_fSpeed = CTempEffect::Get_RandomFloat(1.f, 5.f);
		m_pTransform->m_vInfo[INFO_POS].x = CTempEffect::Get_RandomFloat(m_vecOriginPos.x + m_EffectBoundingBox.vMin.x, m_vecOriginPos.x + m_EffectBoundingBox.vMax.x);
		m_pTransform->m_vInfo[INFO_POS].z = CTempEffect::Get_RandomFloat(m_vecOriginPos.z + m_EffectBoundingBox.vMin.z, m_vecOriginPos.z + m_EffectBoundingBox.vMax.z);
	}

	CTempEffect::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectWaterBubble::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectWaterBubble::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectWaterBubble"));
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

void CEffectWaterBubble::Free()
{
	CTempEffect::Free();
}

CEffectWaterBubble* CEffectWaterBubble::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectWaterBubble*	pInstance = new CEffectWaterBubble(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectWaterBubble Create Failed");
		return nullptr;
	}

	return pInstance;
}
