#include "stdafx.h"
#include "..\Header\EffectStar.h"
#include "PoolManager.h"

CEffectStar::CEffectStar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectStar::~CEffectStar()
{
	Free();
}

HRESULT CEffectStar::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_eObjectTag = OBJECTTAG::IMMORTAL;
	m_EffectTag = EFFECTTAG::EFFECT_STAR;

	if (SceneManager()->Get_Scene()->Get_SceneTag() == SCENETAG::BOSSSTAGE)
	{
		m_pTransform->Translate(_vec3(CTempEffect::Get_RandomFloat(-300.f, 300.f), CTempEffect::Get_RandomFloat(50.f, 150.f), CTempEffect::Get_RandomFloat(-300.f, 300.f)));
	}
	else
		m_pTransform->Translate(_vec3(CTempEffect::Get_RandomFloat(-100.f, 100.f), CTempEffect::Get_RandomFloat(80.f, 150.f), CTempEffect::Get_RandomFloat(-100.f, 100.f)));

	m_fEffectScale = CTempEffect::Get_RandomFloat(.8f, 1.3f);
	m_fScaleTime   = CTempEffect::Get_RandomFloat(0.f, 4.0f);

	return S_OK;
}

_int CEffectStar::Update_Object(const _float& fTimeDelta)
{
	m_fScaleTime += 1.f * fTimeDelta;

	if (m_fScaleTime < 2.f)
	{
		if(m_fEffectScale > 0.f)
			m_fEffectScale -= 1.f * fTimeDelta;
	}
	else
	{
		m_fEffectScale += 1.f * fTimeDelta;
		if (m_fScaleTime > 4.f) {
			m_fScaleTime = 0.f;
		}
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	/*if (m_fTime > m_fLife || m_fFrame == m_fFinal && m_bAnimation && !m_bLoop)
	{
		CPoolManager::GetInstance()->Delete_Object(this);
	}*/

	return iExit;
}

void CEffectStar::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectStar::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);

	if (SceneManager()->Get_Scene()->Get_SceneTag() == SCENETAG::BOSSSTAGE) {
		m_pTextureCom->Render_Texture(2);
	}
	else
		m_pTextureCom->Render_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
}

HRESULT CEffectStar::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Star"));
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

CEffectStar* CEffectStar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectStar* pInstance = new CEffectStar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CEffectStar Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffectStar::Free()
{
	__super::Free();
}
