#include "stdafx.h"
#include "Ring.h"
#include "Export_Function.h"
#include "Player.h" 


CRing::CRing(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CItem(pGraphicDev)
{
}

CRing::CRing(const CRing& rhs)
	: Engine::CItem(rhs)
{
}

CRing::~CRing()
{
	Free();
}

HRESULT CRing::Ready_Object(_bool _Item)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_bWorldItem = _Item;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	if (!Get_WorldItem())
	{
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
		m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
			m_pTransform->LocalScale());
	}
	else
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));

	m_ItemID.eItemType = ITEMTYPE::ITEMTYPE_EQUIPITEM;
	m_ItemID.eItemID = ITEMID::EQUIP_BIGGOLDRING;
	m_ItemID.iCount = 1;

#pragma region Top Stat
	m_pBasicStat->Get_Stat()->iArmorMax = 2.f;
	m_pBasicStat->Get_Stat()->iArmorMin = 1.f;
#pragma endregion

	return S_OK;
}

_int CRing::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CRing::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
}


void CRing::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();
}

HRESULT CRing::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Ring"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	if (Get_WorldItem())
	{
		pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);
	}

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CRing::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);

}

void CRing::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }


	if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionStay(_pOther);
}

void CRing::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }


	if (!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER) &&
		!(_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionExit(_pOther);
}

CRing* CRing::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item)
{
	CRing* pInstance = new CRing(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Item)))
	{
		Safe_Release(pInstance);

		MSG_BOX("TempItem Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CRing::Free()
{
	__super::Free();
}