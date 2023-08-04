#include "stdafx.h"
#include "..\Header\ImmortalSprite.h"

#include "Export_Function.h"

CImmortalSprite::CImmortalSprite(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnvironment(pGraphicDev)
{
}

CImmortalSprite::CImmortalSprite(const CImmortalSprite& rhs)
	: CEnvironment(rhs)
{
}

CImmortalSprite::~CImmortalSprite()
{
}

HRESULT CImmortalSprite::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::IMMORTAL;
	m_eEnvTag = ENVIRONMENTTAG::ETC;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		__super::Ready_Object();

	return S_OK;
}

_int CImmortalSprite::Update_Object(const _float & fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	_uint iExit = 0;
	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		return iExit;

	iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CImmortalSprite::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	//m_pBillBoardCom->LateUpdate_Component();
	//m_pTransform->Scale(_vec3(4.f, 4.f, 4.f));
}

void CImmortalSprite::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTextureCom->Render_Texture(m_iSpriteNumber);
	m_pRcBf->Render_Buffer();

	//////////////////////////////////////////////////////////////////////
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(90.f));
	_matrix matWorld = m_pTransform->WorldMatrix();
	m_pTransform->Rotate(ROT_Y, D3DXToRadian(-90.f));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pTextureCom->Render_Texture(m_iSpriteNumber);
	m_pRcBf->Render_Buffer();
}

HRESULT CImmortalSprite::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pRcBf = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_ImmortalSprite"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CImmortalSprite* CImmortalSprite::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CImmortalSprite*		pInstance = new CImmortalSprite(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CImmortalSprite Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CImmortalSprite::Free()
{
	__super::Free();
}
