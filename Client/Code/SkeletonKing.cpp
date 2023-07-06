#include "stdafx.h"
#include "..\Header\SkeletonKing.h"

#include "Export_Function.h"


CSkeletonKing::CSkeletonKing(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pRcBf(nullptr)
{

}

CSkeletonKing::CSkeletonKing(const CSkeletonKing& rhs)
	: Engine::CGameObject(rhs), m_pRcBf(nullptr)
{
}

CSkeletonKing::~CSkeletonKing()
{
}

HRESULT CSkeletonKing::Ready_Object(void)
{
	return S_OK;
}

_int CSkeletonKing::Update_Object(const _float& fTimeDelta)
{
	return _int();
}

void CSkeletonKing::LateUpdate_Object(void)
{
}

void CSkeletonKing::Render_Object(void)
{
}

HRESULT CSkeletonKing::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pRcBf = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	return S_OK;
}


CSkeletonKing* CSkeletonKing::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkeletonKing* pInstance = new CSkeletonKing(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkeletonKing::Free()
{
	__super::Free();
}
