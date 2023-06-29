#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CPrototypeManager : public CBase
{
	DECLARE_SINGLETON(CPrototypeManager)

private:
	explicit CPrototypeManager();
	virtual ~CPrototypeManager();

public:
	HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent);
	CComponent*		Clone_Proto(const _tchar* pProtoTag);

private:
	CComponent*		Find_Prototype(const _tchar* pProtoTag);

private:
	map<const _tchar*, CComponent*>		m_mapProto;

public:
	virtual void Free();
};

END