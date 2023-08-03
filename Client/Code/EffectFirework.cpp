#include "stdafx.h"
#include "..\Header\EffectFirework.h"
#include "PoolManager.h"
#include <EffectFireworkTrace.h>
#include "SoundManager.h"

CEffectFirework::CEffectFirework(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempEffect(pGraphicDev)
{
}

CEffectFirework::~CEffectFirework()
{
	Free();
}

HRESULT CEffectFirework::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CTempEffect::Ready_Object(), E_FAIL); // 초기화 및 초기 설정
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_EffectTag = EFFECTTAG::EFFECT_FIREWORK;

	// 사운드 재생
	CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_EFFECT);
	CSoundManager::GetInstance()->PlaySound(L"Firework_01", CHANNELID::SOUND_EFFECT, 1.f);

	m_fDistance = 50.f;
	m_fHeight = CTempEffect::Get_RandomFloat(30.f, 80.f);
	m_fSpeed  = CTempEffect::Get_RandomFloat(3.f, 10.f);

	return S_OK;
}

Engine::_int CEffectFirework::Update_Object(const _float& fTimeDelta)
{
	if (m_RandomSet) {
		m_RandomSet = false;
		m_vOriginPos = m_pTransform->m_vInfo[INFO_POS];
		m_pTransform->m_vInfo[INFO_POS].x = CTempEffect::Get_RandomFloat(m_vOriginPos.x - m_fDistance, m_vOriginPos.x + m_fDistance);
		m_pTransform->m_vInfo[INFO_POS].y = CTempEffect::Get_RandomFloat(0.f, 30.f);
		m_pTransform->m_vInfo[INFO_POS].z = CTempEffect::Get_RandomFloat(m_vOriginPos.z - m_fDistance, m_vOriginPos.z + (m_fDistance / 2));
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	// 올라간다
	m_pTransform->m_vInfo[INFO_POS].y += 1.f * fTimeDelta * m_fSpeed;

	// 일정 높이 일시 폭죽 파티클 생성한다
	if (m_pTransform->m_vInfo[INFO_POS].y >= m_fHeight)
	{
		CGameObject* pGameObject = CEffectFireworkTrace::Create(m_pGraphicDev, m_pTransform->m_vInfo[INFO_POS], 1500);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

		// 다시 랜덤 위치 아래로 내려간다.
		m_pTransform->m_vInfo[INFO_POS].x = CTempEffect::Get_RandomFloat(m_vOriginPos.x - m_fDistance, m_vOriginPos.x + m_fDistance);
		m_pTransform->m_vInfo[INFO_POS].y = 0.f;
		m_pTransform->m_vInfo[INFO_POS].z = CTempEffect::Get_RandomFloat(m_vOriginPos.z - m_fDistance, m_vOriginPos.z + (m_fDistance / 2));
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_EFFECT);
		CSoundManager::GetInstance()->PlaySound(L"Firework_01", CHANNELID::SOUND_EFFECT, 1.f);
		m_fHeight = CTempEffect::Get_RandomFloat(30.f, 80.f);
		m_fSpeed = CTempEffect::Get_RandomFloat(3.f, 10.f);
	}

	_int iExit = CTempEffect::Update_Object(fTimeDelta);

	return iExit;
}

void CEffectFirework::LateUpdate_Object(void)
{
	CTempEffect::LateUpdate_Object();

	m_pBillBoardCom->LateUpdate_Component();
	m_pTransform->Scale(_vec3(0.1f, 0.3f, 0.3f));
}

void CEffectFirework::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture((_uint)0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CEffectFirework::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EffectFireWorkMain"));
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

void CEffectFirework::Free()
{
	CTempEffect::Free();
}

CEffectFirework* CEffectFirework::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectFirework* pInstance = new CEffectFirework(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CEffectFirework Create Failed");
		return nullptr;
	}

	return pInstance;
}
