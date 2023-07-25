#include "stdafx.h"
#include "..\Header\Effectleaf.h"
#include "PoolManager.h"

CEffectleaf::CEffectleaf(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectleaf::~CEffectleaf()
{
	Free();
}

HRESULT CEffectleaf::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_LEAF;

	m_fFrame = CTempEffect::Get_RandomFloat(0.f, 2.f);
	m_fEffectScale = CTempEffect::Get_RandomFloat(0.5f, 2.f);
	CTempEffect::Get_RandomVector(&m_pTransform->m_vInfo[INFO_POS], &_vec3(-50.f, 20.f, -50.f), &_vec3(50.f, 100.f, 50.f));

	return S_OK;
}

Engine::_int CEffectleaf::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	m_pTransform->m_vInfo[INFO_POS] += m_vecMoveDir * fTimeDelta * m_bMoveSpeed;

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectleaf::LateUpdate_Object(void)
{
	if (m_pTransform->m_vInfo[INFO_POS].y <= 0.f) {
		m_fFrame = CTempEffect::Get_RandomFloat(0.f, 2.f);
		m_fEffectScale = CTempEffect::Get_RandomFloat(0.5f, 2.f);
		CTempEffect::Get_RandomVector(&m_pTransform->m_vInfo[INFO_POS], &_vec3(-100.f, 90.f, -100.f), &_vec3(100.f, 100.f, 100.f));
	}

	CTempEffect::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(m_fEffectScale, m_fEffectScale, m_fEffectScale));
}

void CEffectleaf::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, & m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectleaf::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectGreenleaves"));
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

void CEffectleaf::Free()
{
	CTempEffect::Free();
}

CEffectleaf* CEffectleaf::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectleaf*	pInstance = new CEffectleaf(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectleaf Create Failed");
		return nullptr;
	}

	return pInstance;
}
