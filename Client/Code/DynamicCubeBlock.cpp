#include "stdafx.h"
#include "DynamicCubeBlock.h"

#include "Export_Function.h"

CDynamicCubeBlock::CDynamicCubeBlock(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCubeBlock(pGraphicDev)
{
	m_byTextureNumber = 31;
}

CDynamicCubeBlock::CDynamicCubeBlock(const CDynamicCubeBlock& rhs)
	: CCubeBlock(rhs)
{
	m_byTextureNumber = 31;
}

CDynamicCubeBlock::~CDynamicCubeBlock()
{
}

HRESULT CDynamicCubeBlock::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

#ifdef _DEBUG
	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
	{
		void* pVertices = nullptr;
		m_pBuffer->m_pVB->Lock(0, m_pBuffer->m_dwVtxCnt * m_pBuffer->m_dwVtxSize, &pVertices, 0);
		for (UINT i = 0; i < m_pBuffer->m_dwVtxCnt; ++i)
		{
			m_vecCubeVertex.push_back((((VTXNTX*)pVertices) + i)->vPosition);
		}
		m_pBuffer->m_pVB->Unlock();

		void* pIndices = nullptr;
		m_pBuffer->m_pIB->Lock(0, m_pBuffer->m_dwTriCnt * m_pBuffer->m_dwIdxSize, &pIndices, 0);
		for (UINT i = 0; i < m_pBuffer->m_dwTriCnt; ++i)
		{
			m_vecCubeIndex.push_back(*(((INDEX32*)pIndices) + i));
		}
		m_pBuffer->m_pIB->Unlock();
	}
#endif _DEBUG

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	return S_OK;
}

_int CDynamicCubeBlock::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	//Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CDynamicCubeBlock::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CDynamicCubeBlock::Render_Object(void)
{
	//WaterFlow();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	/*if (31 == m_byTextureNumber)
		++m_byTextureNumber;
	else
		--m_byTextureNumber;*/

	m_pTexture->Render_Texture(m_byTextureNumber);
	m_pBuffer->Render_Buffer();
}

HRESULT CDynamicCubeBlock::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = static_cast<CDynamicBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_DynamicBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Cube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CDynamicCubeBlock::WaterFlow()
{
	// 버퍼 잠그기
	//void* pVertices = nullptr;
	//m_pBuffer->m_pVB->Lock(0, m_pBuffer->m_dwVtxCnt * m_pBuffer->m_dwVtxSize, &pVertices, 0);
	//for (UINT i = 0; i < m_pBuffer->m_dwVtxCnt; ++i)
	//{
	//	m_vecCubeVertex.push_back((((VTXNTX*)pVertices) + i)->vPosition);
	//}
	//m_pBuffer->m_pVB->Unlock();


	void* pVertices = nullptr;
	m_pBuffer->m_pVB->Lock(0, m_pBuffer->m_dwVtxCnt * m_pBuffer->m_dwVtxSize, &pVertices, 0);

	// UV 좌표를 매프레임마다 조절하여 물의 흐름 효과를 만듭니다.
	// 여기서는 간단히 예시를 들기 위해 시간에 따라 변하는 값으로 가정합니다.
	// 실제로는 물의 흐름 패턴을 적용하고자 할 것입니다.

	// 예시: UV 좌표를 시간에 따라 변화시킵니다.
	for (UINT i = 0; i < 8; ++i)
	{
		VTXNTX* pVertex = reinterpret_cast<VTXNTX*>(static_cast<_ubyte*>(pVertices) + i * sizeof(VTXNTX));
		//pVertex->u = fmodf(pVertex->u + time * speedU, 1.0f); // U 좌표 수정

		_float* fU = ((_float*)&(pVertex->vTexUV) + 0);
		*fU += (1.f < *fU ? -1.999f : 0.001f);
	}

	// 버퍼 잠금 해제
	m_pBuffer->m_pVB->Unlock();
}

CDynamicCubeBlock* CDynamicCubeBlock::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicCubeBlock* pInstance = new CDynamicCubeBlock(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("DynamicCubeBlock Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDynamicCubeBlock::Free()
{
	__super::Free();
}
