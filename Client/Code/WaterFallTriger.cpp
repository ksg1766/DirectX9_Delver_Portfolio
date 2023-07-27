#include "stdafx.h"
#include "SoundManager.h"
#include "Export_Function.h"
#include "Player.h"
#include "..\Include\WaterFallTriger.h"

CWaterFallTriger::CWaterFallTriger(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CWaterFallTriger::CWaterFallTriger(const CWaterFallTriger& rhs)
	: Engine::CGameObject(rhs)
{
}

CWaterFallTriger::~CWaterFallTriger()
{
}

HRESULT CWaterFallTriger::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::TRIGERBLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(4.f, 20.f, 4.f));
	m_pTransform->Translate(_vec3(-43.f, 30.f, 19.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale());


	m_fSound = 0.f;
	return S_OK;
}

_int CWaterFallTriger::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	_uint iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CWaterFallTriger::LateUpdate_Object()
{
	__super::LateUpdate_Object();

	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&vDir);


	_float fVolume = m_fMaxVolume - (fDistance / m_fMaxDistance) * (m_fMaxVolume - m_fMinVolume);

	if (fVolume <= 0.f)
		m_fSound = 0.f;
	else
		m_fSound = fVolume;

	CSoundManager::GetInstance()->PlaySound(L"env_waterfall.mp3", CHANNELID::SOUND_ENVIRONMENT, m_fSound);
}

void CWaterFallTriger::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	//m_pTexture->Render_Texture(0);
	m_pCubeBf->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif // _DEBUG

}

void CWaterFallTriger::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);
}

void CWaterFallTriger::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);
}

void CWaterFallTriger::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionExit(_pOther);
}

HRESULT CWaterFallTriger::Add_Component()
{
	CComponent* pComponent = nullptr;
	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_VillageTiger"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CWaterFallTriger* CWaterFallTriger::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterFallTriger* pInstance = new CWaterFallTriger(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CWaterFallTriger*>(pInstance);

		MSG_BOX("Create WaterFallTriger FAILED");
		return nullptr;
	}

	return pInstance;
}

void CWaterFallTriger::Free()
{
	__super::Free();
}
