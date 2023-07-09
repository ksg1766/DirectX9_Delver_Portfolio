#include "..\..\Header\GameObject.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_IsDead(false), m_eObjectTag(OBJECTTAG::OBJECT_END)
{
	m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev), m_IsDead(false), m_eObjectTag(OBJECTTAG::OBJECT_END)
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}

CComponent * CGameObject::Get_Component(COMPONENTTAG eComponentTag, COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(eComponentTag, eID);

	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

HRESULT CGameObject::Ready_Object(void)
{
	return S_OK;
}

_int CGameObject::Update_Object(const _float & fTimeDelta)
{
	//for (auto& iter : m_mapComponent[ID_DYNAMIC])
	//	iter.second->Update_Component(fTimeDelta);
	
	return 0;
}

void CGameObject::LateUpdate_Object(void)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component();
}


CComponent * CGameObject::Find_Component(COMPONENTTAG eComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_FinderEnum<COMPONENTTAG>(eComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}
