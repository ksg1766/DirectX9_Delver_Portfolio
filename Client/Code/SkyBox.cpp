#include "stdafx.h"
#include "..\Header\SkyBox.h"

#include "Export_Function.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSkyBox::CSkyBox(const CSkyBox & rhs)
	: CGameObject(rhs)
{
}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eObjectTag = OBJECTTAG::SKYBOX;

	m_pTransformCom->m_vScale = { 40.f, 40.f, 40.f };

	return S_OK;
}

_int CSkyBox::Update_Object(const _float & fTimeDelta)
{

	_int iExit = __super::Update_Object(fTimeDelta);

	Engine::Renderer()->Add_RenderGroup(RENDER_PRIORITY, this);

	return iExit;
}

void CSkyBox::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	m_pTransformCom->Set_Pos(matView._41, matView._42 + 3.f, matView._43);


}

void CSkyBox::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Render_Texture(3);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSkyBox::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkyBox"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE, pComponent);

	return S_OK;
}

CSkyBox * CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox *		pInstance = new CSkyBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("SkyBox Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkyBox::Free()
{
	__super::Free();
}
