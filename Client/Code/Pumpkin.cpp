#include "stdafx.h"
#include "..\Header\Pumpkin.h"

#include "Export_Function.h"

CPumpkin::CPumpkin(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnvironment(pGraphicDev)
{
}

CPumpkin::CPumpkin(const CPumpkin& rhs)
	: CEnvironment(rhs)
{
}

CPumpkin::~CPumpkin()
{
}

HRESULT CPumpkin::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::IMMORTAL;
	m_eEnvTag = ENVIRONMENTTAG::PUMPKIN;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		__super::Ready_Object();

	return S_OK;
}

_int CPumpkin::Update_Object(const _float & fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_uint iExit = 0;
	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		return iExit;

	iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CPumpkin::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	//m_pBillBoardCom->LateUpdate_Component();
	//m_pTransform->Scale(_vec3(4.f, 4.f, 4.f));
}

void CPumpkin::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture(m_iPumpkinNumber);
	m_pRcBf->Render_Buffer();

	//////////////////////////////////////////////////////////////////////
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(90.f));
	_matrix matWorld = m_pTransform->WorldMatrix();
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(-90.f));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pTextureCom->Render_Texture(m_iPumpkinNumber);
	m_pRcBf->Render_Buffer();
}

HRESULT CPumpkin::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pRcBf = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_VillagePumpkin"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CPumpkin* CPumpkin::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPumpkin*		pInstance = new CPumpkin(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CRock Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPumpkin::Free()
{
	__super::Free();
}
