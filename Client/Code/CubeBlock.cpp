#include "stdafx.h"
#include "CubeBlock.h"

#include "Export_Function.h"

CCubeBlock::CCubeBlock(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev), m_byTextureNumber(7)
{
}

CCubeBlock::CCubeBlock(const CCubeBlock& rhs)
    : CGameObject(rhs), m_byTextureNumber(7)
{
}

CCubeBlock::~CCubeBlock()
{
}

HRESULT CCubeBlock::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

#ifdef _DEBUG
	//if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
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

	//m_pTransform->Scale(_vec3(2.f, 2.f, 2.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	//m_pTransform->Translate(_vec3(0.f, 2.f, 0.f));
	
	return S_OK;
}

_int CCubeBlock::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	
	return iExit;
}

void CCubeBlock::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CCubeBlock::Render_Object(void)
{
	//D3DLIGHT9 light;
	//ZeroMemory(&light, sizeof(D3DLIGHT9));
	//light.Type = D3DLIGHT_DIRECTIONAL;
	//light.Diffuse = D3DXCOLOR( 1.f, 1.f, 1.f, 1.f );
	//light.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//light.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//_vec3 vDir(1.f, -1.f, -1.f);
	//D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vDir);
	//light.Range=1000.f;

	//m_pGraphicDev->SetLight(0, &light);
	//m_pGraphicDev->LightEnable(0, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(0, 255, 255, 255));
	//D3DMATERIAL9 mtrl;
	//ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	//mtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//mtrl.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	//mtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//mtrl.Emissive = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	//mtrl.Power = 1.f;
	//m_pGraphicDev->SetMaterial(&mtrl);


	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pTexture->Render_Texture(m_byTextureNumber);
	m_pBuffer->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

#if _DEBUG
	//m_pCollider->Render_Collider();
#endif _DEBUG

}

void CCubeBlock::OnCollisionEnter(CCollider* _pOther)
{

}

void CCubeBlock::OnCollisionStay(CCollider* _pOther)
{
}

void CCubeBlock::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CCubeBlock::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Cube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	/*pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);*/

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CCubeBlock* CCubeBlock::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeBlock* pInstance = new CCubeBlock(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CubeBlock Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CCubeBlock::Free()
{
	__super::Free();
}
