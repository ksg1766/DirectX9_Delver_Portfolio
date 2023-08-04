#include "stdafx.h"
#include "..\Header\EffectWaterfall.h"
#include "PoolManager.h"
#include "EffectWaterMove.h"
#include "EffectWaterBubble.h"
#include "EffectWater.h"

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

	m_fChangeTime = CTempEffect::Get_RandomFloat(1.f, 2.f);

	return S_OK;
}

Engine::_int CEffectWaterfall::Update_Object(const _float& fTimeDelta)
{
	if (m_RandomSet) {
		m_RandomSet = false;

		if (m_iMoveSet != 0) {
			// WaterMove 생성
			for (_uint i = 0; i < m_iMoveCount; ++i) {
				CGameObject* pGameObject = CEffectWaterMove::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vInfo[INFO_POS].z));
				static_cast<CEffectWaterMove*>(pGameObject)->Set_OriginPos(m_pTransform->m_vInfo[INFO_POS]);
				static_cast<CEffectWaterMove*>(pGameObject)->Set_BoundingBox(m_EffectBoundingBox.vMin, m_EffectBoundingBox.vMax);
				static_cast<CEffectWaterMove*>(pGameObject)->Set_EffectMoveScale(m_vecMoveScale[0], m_vecMoveScale[1]);
				static_cast<CEffectWaterMove*>(pGameObject)->Set_PickNumber(i, m_iMoveCount);
				static_cast<CEffectWaterMove*>(pGameObject)->Set_EffectMoveSet(m_iMoveSet);

				m_vecMove.push_back(pGameObject);
			}
		}

		// WaterBubble 생성
		for (_uint i = 0; i < m_iBubbleCount; ++i) {
			CGameObject* pGameObject = CEffectWaterBubble::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pGameObject->m_pTransform->Translate(_vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vInfo[INFO_POS].z));
			static_cast<CEffectWaterBubble*>(pGameObject)->Set_OriginPos(m_pTransform->m_vInfo[INFO_POS]);
			static_cast<CEffectWaterBubble*>(pGameObject)->Set_BoundingBox(m_EffectBoundingBox.vMin, m_EffectBoundingBox.vMax);
			static_cast<CEffectWaterBubble*>(pGameObject)->Set_EffectBubbleScale(m_vecBubbleScale[0], m_vecBubbleScale[1]);
			m_vecBubble.push_back(pGameObject);
		}
	}

	// 랜덤 초마다 튀는 물 생성 // 생기는 개수 랜덤
	m_fMoveTime += 1.f * fTimeDelta;

	if (m_iMoveSet != 0 && m_fMoveTime > m_fChangeTime)
	{
		m_fMoveTime = 0.f;
		m_fChangeTime     = CTempEffect::Get_RandomFloat(1.f, 2.f);
		m_iParticleNumber = (int)CTempEffect::Get_RandomFloat(30.f, 60.f);

		CGameObject* pGameObject = CEffectWater::Create(m_pGraphicDev, _vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y + m_fHeight, m_pTransform->m_vInfo[INFO_POS].z), m_iParticleNumber, 0.05f, EFFECTCOLOR::ECOLOR_WHITE);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	}

	for (auto& iter : m_vecMove)
		iter->Update_Object(fTimeDelta);

	for (auto& iter : m_vecBubble)
		iter->Update_Object(fTimeDelta);

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectWaterfall::LateUpdate_Object(void)
{
	for (auto& iter : m_vecMove)
		iter->LateUpdate_Object();

	for (auto& iter : m_vecBubble)
		iter->LateUpdate_Object();

	CTempEffect::LateUpdate_Object();
}

void CEffectWaterfall::Render_Object(void)
{
	for (auto& iter : m_vecMove)
		iter->Render_Object();

	for (auto& iter : m_vecBubble)
		iter->Render_Object();
}

HRESULT CEffectWaterfall::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectBlood"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CEffectWaterfall::Free()
{
	for_each(m_vecDrop.begin(), m_vecDrop.end(), CDeleteObj());
	m_vecDrop.clear();

	for_each(m_vecMove.begin(), m_vecMove.end(), CDeleteObj());
	m_vecMove.clear();

	for_each(m_vecBubble.begin(), m_vecBubble.end(), CDeleteObj());
	m_vecBubble.clear();

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
