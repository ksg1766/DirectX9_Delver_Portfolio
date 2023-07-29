#include "stdafx.h"
#include "SoundManager.h"
#include "VillageTriger.h"
#include "Export_Function.h"
#include "Player.h"
#include "Stage.h"

CVillageTriger::CVillageTriger(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CVillageTriger::CVillageTriger(const CVillageTriger& rhs)
	:Engine::CGameObject(rhs)
{
}

CVillageTriger::~CVillageTriger()
{
}

HRESULT CVillageTriger::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::TRIGERBLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(4.f, 4.f, 4.f));
	m_pTransform->Translate(_vec3(-101.f, 6.f, -27.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());


	return S_OK;
}

_int CVillageTriger::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	_uint iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CVillageTriger::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	CGameObject* pPlayer = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();

	if (pPlayer != nullptr)
	{
		_vec3 vDir = pPlayer->m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
		_float fDistance = D3DXVec3Length(&vDir);

		if (fDistance < 5.f)
		{
			// 마을 -> 스테이지 씬 이동
			CSoundManager::GetInstance()->PlaySound(L"door_beginning.mp3", CHANNELID::SOUND_ENVIRONMENT, 1.f);

			CScene* pScene = CStage::Create(m_pGraphicDev);
			Engine::SceneManager()->Change_Scene(pScene);
		}
	}
}

void CVillageTriger::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(0);
	m_pCubeBf->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CVillageTriger::OnCollisionEnter(CCollider* _pOther)
{
	__super::OnCollisionEnter(_pOther);

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER)
	{
		//_uint i = 0;
	}
}

void CVillageTriger::OnCollisionStay(CCollider* _pOther)
{
	__super::OnCollisionStay(_pOther);
}

void CVillageTriger::OnCollisionExit(CCollider* _pOther)
{
	__super::OnCollisionExit(_pOther);
}

HRESULT CVillageTriger::Add_Component(void)
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

CVillageTriger* CVillageTriger::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVillageTriger* pInstance = new CVillageTriger(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CSceneTriger Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CVillageTriger::Free()
{
	__super::Free();
}
