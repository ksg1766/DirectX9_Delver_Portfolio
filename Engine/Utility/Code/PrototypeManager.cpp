#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CPrototypeManager)

CPrototypeManager::CPrototypeManager()
{
}

CPrototypeManager::~CPrototypeManager()
{
	Free();
}

HRESULT CPrototypeManager::Ready_Proto(const _tchar * pProtoTag, CComponent * pComponent)
{
	CComponent*		pPrototype = Find_Prototype(pProtoTag);

	if (nullptr != pPrototype)
		return E_FAIL;

	m_mapProto.emplace(pProtoTag, pComponent);

	return S_OK;
}

CComponent * CPrototypeManager::Clone_Proto(const _tchar * pProtoTag)
{
	CComponent* pPrototype = Find_Prototype(pProtoTag);

	NULL_CHECK_RETURN(pPrototype, nullptr);

	return pPrototype->Clone();
}

CComponent * CPrototypeManager::Find_Prototype(const _tchar * pProtoTag)
{
	auto		iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapProto.end())
		return nullptr;
	
	return iter->second;
}

void CPrototypeManager::Free()
{
	for_each(m_mapProto.begin(), m_mapProto.end(), CDeleteMap());
	m_mapProto.clear();
}
