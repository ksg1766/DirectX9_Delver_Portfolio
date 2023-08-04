#include "stdafx.h"
#include "Bonfire.h"
#include "Export_Function.h"
#include "EffectBonfire.h"
#include "Fire.h"
#include "SoundManager.h"
#include "Player.h"

CBonfire::CBonfire(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBonfire::CBonfire(const CBonfire& rhs)
	:Engine::CGameObject(rhs)
{
}

CBonfire::~CBonfire()
{
}

HRESULT CBonfire::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::IMMORTAL;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Translate(_vec3(0.f, 3.f, -22.f));

	//CGameObject* pGameObject = CFire::Create(m_pGraphicDev);
	//Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	//
	//// 불 이펙트 추가 생성
	//for (_uint i = 0; i < 4; ++i){
	//	pGameObject = CEffectBonfire::Create(m_pGraphicDev);
	//	pGameObject->m_pTransform->Translate(_vec3(0.f, 1.f * i, 0.f));
	//	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	//}

	m_fTime = 0.f;

	return S_OK;
}

_int CBonfire::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	_uint iExit = __super::Update_Object(fTimeDelta);

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&vDir);

	if (fDistance < 5.f)
	{
		m_fTime += fTimeDelta;

		if (m_fTime < 14.f)
			CSoundManager::GetInstance()->PlaySound(L"torch.mp3", CHANNELID::SOUND_ENVIRONMENT, 1.f);
		else
			m_fTime = 0.f;
	}
	else
	{
		m_fTime = 0.f;
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_ENVIRONMENT);
	}


	return iExit;
}

void CBonfire::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBonfire::Render_Object(void)
{
	m_pTransform->Scale(_vec3(1.f, 0.2f, 1.f));
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(0);
	m_pCubeBf->Render_Buffer();

	m_pTransform->Scale(_vec3(.5f, 0.1f, .5f));
	m_pTransform->Translate(_vec3(0.f, .2f, 0.f));
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(1);
	m_pCubeBf->Render_Buffer();

	m_pTransform->Translate(_vec3(0.f, -.2f, 0.f));
}

HRESULT CBonfire::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pCubeBf = static_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Bonfire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBonfire* CBonfire::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBonfire* pInstance = new CBonfire(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CBonfire Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBonfire::Free()
{
	__super::Free();
}
