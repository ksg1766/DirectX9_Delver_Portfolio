#include "stdafx.h"
#include "..\Header\SkeletonKing_Clone.h"
#include "Export_Function.h"
#include "BossProjectile.h"
#include "BossExplosion.h"

CSkeletonKing_Clone::CSkeletonKing_Clone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{

}

CSkeletonKing_Clone::CSkeletonKing_Clone(const CSkeletonKing_Clone& rhs)
	: Engine::CMonster(rhs)
{
}

CSkeletonKing_Clone::~CSkeletonKing_Clone()
{
	Free();
}

HRESULT CSkeletonKing_Clone::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BOSS;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));
	return S_OK;
}

_int CSkeletonKing_Clone::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);
	
	m_fFrame += 8.f * fTimeDelta;
	m_fSkillCool += fTimeDelta;
	if (8.f < m_fFrame)
		m_fFrame = 0.f;

	if (2.5f < m_fSkillCool)
	{
		Engine::CGameObject* pGameObject = nullptr;
		pGameObject = CBossProjectile::Create(m_pGraphicDev);
		dynamic_cast<CBossProjectile*>(pGameObject)->Set_Target(m_pTransform->m_vInfo[INFO_POS]);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_fSkillCool = 0.f;
	}
	return iExit;
}

void CSkeletonKing_Clone::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));
}

void CSkeletonKing_Clone::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture((_uint)m_fFrame);
	m_pBuffer->Render_Buffer();
}

HRESULT CSkeletonKing_Clone::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Boss"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CSkeletonKing_Clone* CSkeletonKing_Clone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkeletonKing_Clone* pInstance = new CSkeletonKing_Clone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SkeletonKing Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkeletonKing_Clone::Free()
{
	__super::Free();
}

void CSkeletonKing_Clone::Init_Stat()
{
}