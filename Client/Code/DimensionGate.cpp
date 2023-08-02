#include "stdafx.h"
#include "DimensionGate.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Player.h"
#include "SoundManager.h"
#include "CameraManager.h"
#include "FlyingCamera.h"
#include "Village.h"

CDimensionGate::CDimensionGate(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CDimensionGate::CDimensionGate(const CDimensionGate& rhs)
	: Engine::CGameObject(rhs)
{
}

CDimensionGate::~CDimensionGate()
{
	Free();
}

HRESULT CDimensionGate::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::MONSTER;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fTime = 0.f;
	m_fScale = 1.;
	m_fFrame = 0.f;
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS]+_vec3(-0.5f, 0.f, -0.5f), &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	return S_OK;
}

_int CDimensionGate::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);
	m_fFrame += 5.f * fTimeDelta;
	if (5.f < m_fFrame)
		m_fFrame = 0.f;
	//if (35.f >= m_pTransform->m_vInfo[INFO_POS].y)
	//	m_pTransform->Translate(_vec3(0.f, fTimeDelta, 0.f));
	return iExit;
}

void CDimensionGate::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CDimensionGate::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pTexture->Render_Texture(_int(m_fFrame));
	m_pBuffer->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CDimensionGate::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (OBJECTTAG::PLAYER == _pOther->Get_Host()->Get_ObjectTag())
	{
		CSoundManager::GetInstance()->PlaySound(L"door_beginning.mp3", CHANNELID::SOUND_ENVIRONMENT, 1.f);

		SceneManager()->Get_Scene()->Get_MainPlayer()->Get_RigidBody()->Set_Force(_vec3(0.f, 0.f, 0.f));
		CScene* pScene = CVillage::Create(m_pGraphicDev);
		Engine::SceneManager()->Change_Scene(pScene);
		//CFlyingCamera* pCamera = static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam());
		//pCamera->m_pTransform->Copy_RUL_AddPos(SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo);
		//static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Change_Mode();
	}
}

void CDimensionGate::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	
}

void CDimensionGate::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CDimensionGate::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossPortal"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CDimensionGate* CDimensionGate::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDimensionGate* pInstance = new CDimensionGate(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("DimensionGate Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDimensionGate::Free()
{
	__super::Free();
}
