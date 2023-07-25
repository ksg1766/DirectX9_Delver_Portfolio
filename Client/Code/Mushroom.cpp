#include "stdafx.h"
#include "..\Header\Mushroom.h"

#include "Export_Function.h"

CMushroom::CMushroom(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnvironment(pGraphicDev)
{
}

CMushroom::CMushroom(const CMushroom& rhs)
	: CEnvironment(rhs)
{
}

CMushroom::~CMushroom()
{
}

HRESULT CMushroom::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::IMMORTAL;
	m_eEnvTag = ENVIRONMENTTAG::MUSHROOM;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		__super::Ready_Object();

	return S_OK;
}

_int CMushroom::Update_Object(const _float & fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_uint iExit = 0;
	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		return iExit;

	iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CMushroom::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	//m_pBillBoardCom->LateUpdate_Component();
	//m_pTransform->Scale(_vec3(4.f, 4.f, 4.f));
}

void CMushroom::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture(m_iMushroomNumber);
	m_pRcBf->Render_Buffer();

	//////////////////////////////////////////////////////////////////////
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(90.f));
	_matrix matWorld = m_pTransform->WorldMatrix();
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(-90.f));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pTextureCom->Render_Texture(m_iMushroomNumber);
	m_pRcBf->Render_Buffer();
}

HRESULT CMushroom::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pRcBf = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_VillageMushroom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CMushroom* CMushroom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMushroom*		pInstance = new CMushroom(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CMushroom Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMushroom::Free()
{
	__super::Free();
}
