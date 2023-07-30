#include "stdafx.h"
#include "..\Header\EffectWaterfall.h"
#include "PoolManager.h"
#include "EffectWaterMove.h"

CEffectWaterfall::CEffectWaterfall(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectWaterfall::~CEffectWaterfall()
{
	Free();
}

HRESULT CEffectWaterfall::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_WATERFALL;

	return S_OK;
}

Engine::_int CEffectWaterfall::Update_Object(const _float& fTimeDelta)
{
	if (m_RandomSet) {
		m_RandomSet = false;

		// WaterMove 생성
		for (_uint i = 0; i < m_iMoveCount; ++i) {
			CGameObject* pGameObject = CEffectWaterMove::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y + 3.f, m_pTransform->m_vInfo[INFO_POS].z));
			m_vecMove.push_back(pGameObject);
			//Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

			//pGameObject = CEffectWaterMove::Create(m_pGraphicDev);
			//NULL_CHECK_RETURN(pGameObject, E_FAIL);
			//pGameObject->m_pTransform->Translate(_vec3(0.f, 10.f, -30.f));
			//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
		}
	}

	for (auto& iter : m_vecMove)
		iter->Update_Object(fTimeDelta);

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectWaterfall::LateUpdate_Object(void)
{
	for (auto& iter : m_vecMove)
		iter->LateUpdate_Object();

	CTempEffect::LateUpdate_Object();
}

void CEffectWaterfall::Render_Object(void)
{
	for (auto& iter : m_vecMove)
		iter->Render_Object();
}

HRESULT CEffectWaterfall::Add_Component(void)
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

void CEffectWaterfall::Free()
{
	CTempEffect::Free();
}

CEffectWaterfall* CEffectWaterfall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectWaterfall*	pInstance = new CEffectWaterfall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectWaterfall Create Failed");
		return nullptr;
	}

	return pInstance;
}
