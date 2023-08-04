#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpiderGrabWeb.h"
#include "Export_Function.h"
#include "Player.h"

CKingSpiderGrabWeb::CKingSpiderGrabWeb(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMonster(pGraphicDev)
{
}

CKingSpiderGrabWeb::CKingSpiderGrabWeb(const CKingSpiderGrabWeb& rhs)
	: Engine::CMonster(rhs)
{
}

CKingSpiderGrabWeb::~CKingSpiderGrabWeb()
{
	Free();
}

HRESULT CKingSpiderGrabWeb::Ready_Object(void)
{
	Set_ObjectTag(OBJECTTAG::MONSTERBULLET);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
	m_bHit = false;
	m_vfDistance = 0.f;
	return S_OK;
}

_int CKingSpiderGrabWeb::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_PRIORITY, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CKingSpiderGrabWeb::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(0.1f, 0.1f, m_vfDistance));
}

void CKingSpiderGrabWeb::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pCubeBf->Render_Buffer();
}

void CKingSpiderGrabWeb::Init_Stat()
{
}

void CKingSpiderGrabWeb::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::PLAYER)
	__super::OnCollisionEnter(_pOther);
}

void CKingSpiderGrabWeb::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if ((_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
	{

	}
}

void CKingSpiderGrabWeb::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if ((_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
	{

	}
}

void CKingSpiderGrabWeb::Set_Dir(_vec3 _vDir)
{
	m_vDir = _vDir - m_pTransform->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}

HRESULT CKingSpiderGrabWeb::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pCubeBf = static_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	//pComponent = m_pTexture = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_KingSpiderGrab"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = static_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CKingSpiderGrabWeb* CKingSpiderGrabWeb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CKingSpiderGrabWeb* pInstance = new CKingSpiderGrabWeb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("KingSpider GrabWeb Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CKingSpiderGrabWeb::Free()
{
}
