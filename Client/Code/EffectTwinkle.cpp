#include "stdafx.h"
#include "..\Header\EffectTwinkle.h"
#include "PoolManager.h"

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
	m_bLoop      = true;

	m_fFrame = CTempEffect::Get_RandomFloat(0.f, 3.f);
	m_fFrist = 0.f;
	m_fFinal = 3.f;
	m_fFrameSpeed  = 1.5f;

	m_fLife        = 50.f;
	m_fEffectScale = CTempEffect::Get_RandomFloat(.1f, .6f);

	m_fDistance = .8f;

	return S_OK;
}

Engine::_int CEffectTwinkle::Update_Object(const _float& fTimeDelta)
{
	if (m_RandomSet) {
		m_RandomSet = false;
		m_vecOriginPos = m_pTransform->m_vInfo[INFO_POS];
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	//if (m_fTime > m_fLife || m_fFrame == m_fFinal && m_bAnimation && !m_bLoop)
	//{
	//	CPoolManager::GetInstance()->Delete_Object(this);
	//}

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	if (m_bAniEnd) {
		m_bAniEnd = false;
		m_pTransform->Translate(_vec3(CTempEffect::Get_RandomFloat(-.5f, .5f), CTempEffect::Get_RandomFloat(-.5f, .5f), CTempEffect::Get_RandomFloat(-.5f, .5f)));
		// 범위를 어떻게 지정할 수 있을 까?

		if (m_pTransform->m_vInfo[INFO_POS].x > m_vecOriginPos.x + m_fDistance)
		{														   
			m_pTransform->m_vInfo[INFO_POS].x = m_vecOriginPos.x + m_fDistance;
		}														  
		if (m_pTransform->m_vInfo[INFO_POS].x < m_vecOriginPos.x - m_fDistance)
		{														  
			m_pTransform->m_vInfo[INFO_POS].x = m_vecOriginPos.x - m_fDistance;
		}														   

		if (m_pTransform->m_vInfo[INFO_POS].y > m_vecOriginPos.y + m_fDistance)
		{														  
			m_pTransform->m_vInfo[INFO_POS].y = m_vecOriginPos.y + m_fDistance;
		}														   
		if (m_pTransform->m_vInfo[INFO_POS].y < m_vecOriginPos.y - m_fDistance)
		{														   
			m_pTransform->m_vInfo[INFO_POS].y = m_vecOriginPos.y - m_fDistance;
		}														 

		if (m_pTransform->m_vInfo[INFO_POS].z > m_vecOriginPos.z + m_fDistance)
		{														  
			m_pTransform->m_vInfo[INFO_POS].z = m_vecOriginPos.z + m_fDistance;
		}														 
		if (m_pTransform->m_vInfo[INFO_POS].z < m_vecOriginPos.z - m_fDistance)
		{														  
			m_pTransform->m_vInfo[INFO_POS].z = m_vecOriginPos.z - m_fDistance;
		}

		m_fEffectScale = CTempEffect::Get_RandomFloat(.1f, .6f);
	}

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

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectDebuff"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBillBoardCom = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
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
