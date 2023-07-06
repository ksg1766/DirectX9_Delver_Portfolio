#include "stdafx.h"
#include "..\Header\Terrain.h"

#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}
CTerrain::CTerrain(const CTerrain& rhs)
	: Engine::CGameObject(rhs)
{

}
CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::TERRAIN;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	void*		pVertices = nullptr;
	m_pBuffer->m_pVB->Lock(0, m_pBuffer->m_dwVtxCnt * m_pBuffer->m_dwVtxSize, &pVertices, 0);
	for (UINT i = 0; i < m_pBuffer->m_dwVtxCnt; ++i)
	{
		m_vecTerrainVertex.push_back((((VTXNTX*)pVertices) + i)->vPosition);
	}
	m_pBuffer->m_pVB->Unlock();


	return S_OK;
}

Engine::_int CTerrain::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CTerrain::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();
}

void CTerrain::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(0);
	m_pBuffer->Render_Buffer();
}

HRESULT CTerrain::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CTerrainTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	return S_OK;
}



void CTerrain::Free()
{
	__super::Free();
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}

	return pInstance;
}
