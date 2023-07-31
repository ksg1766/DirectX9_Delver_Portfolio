#include "stdafx.h"
#include "..\Header\SkyBoxVillage.h"

#include "Export_Function.h"

CSkyBoxVillage::CSkyBoxVillage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSkyBoxVillage::CSkyBoxVillage(const CSkyBoxVillage& rhs)
	: CGameObject(rhs)
{
}

CSkyBoxVillage::~CSkyBoxVillage()
{
}

HRESULT CSkyBoxVillage::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::SKYBOX;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(400.f, 400.f, 400.f));

	return S_OK;
}

_int CSkyBoxVillage::Update_Object(const _float & fTimeDelta)
{

	_int iExit = __super::Update_Object(fTimeDelta);

	Engine::Renderer()->Add_RenderGroup(RENDER_PRIORITY, this);

	return iExit;
}

void CSkyBoxVillage::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	::CopyMemory(m_pTransform->m_vInfo[INFO_POS], matView.m[3], sizeof(_vec3));
	//m_pTransformCom->m_vInfo[INFO_POS] = _vec3(matView._41, matView._42 + 3.f, matView._43);


}

void CSkyBoxVillage::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);

	m_pTexture->Render_Texture(0);
	m_pCubeBf->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSkyBoxVillage::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_VillageSkyBox"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	return S_OK;
}

CSkyBoxVillage* CSkyBoxVillage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBoxVillage*		pInstance = new CSkyBoxVillage(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CSkyBoxVillage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkyBoxVillage::Free()
{
	__super::Free();
}
