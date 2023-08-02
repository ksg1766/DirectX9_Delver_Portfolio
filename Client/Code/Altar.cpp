#include "..\Header\Altar.h"
#include "Export_Function.h"
#include "Orb.h"
#include "Player.h"

CAltar::CAltar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCubeBlock(pGraphicDev)
{
}

CAltar::CAltar(const CAltar& rhs)
	: CCubeBlock(rhs)
{
}

CAltar::~CAltar()
{
	
}

HRESULT CAltar::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::BLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(1.f, 0.3f, 1.f));
	//m_pTransform->Translate(_vec3(0.f, 4.f, -60.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(0.f, 3.f, -12.f));

	return S_OK;
}

_int CAltar::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	_uint iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CAltar::LateUpdate_Object()
{
	__super::LateUpdate_Object();
}

void CAltar::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(45);
	m_pCubeBf->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif // _DEBUG
}

void CAltar::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::ITEM)
	{
		if (dynamic_cast<CItem*>(_pOther->Get_Host())->Get_ItemTag().eItemID == ITEMID::QUEST_ORB)
		{
			_vec3 vPos = m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, 1.f, 0.f);
			_pOther->Get_Host()->m_pTransform->m_vInfo[INFO_POS] = vPos;

			dynamic_cast<COrb*>(_pOther->Get_Host())->Set_Altar(true);

			CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();
			rPlayer.Set_Orb(true);
		}
	}
}

void CAltar::OnCollisionStay(CCollider* _pOther)
{
}

void CAltar::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CAltar::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Cube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);


	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CAltar* CAltar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAltar* pInstance = new CAltar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CAltar*>(pInstance);

		MSG_BOX("Create Altar FAILED");
		return nullptr;
	}

	return pInstance;
}

void CAltar::Free()
{
	__super::Free();
}
