#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CCollider;
class ENGINE_DLL CCollisionManager :
	public CBase
{
	DECLARE_SINGLETON(CCollisionManager)

private:
	explicit CCollisionManager();
	virtual ~CCollisionManager();

public:
	void LateUpdate_Collision();

public:
	map<ULONGLONG, bool>& GetMapColInfo() { return m_mapColInfo; }
	void SetMapColInfo(ULONGLONG ID, bool bCollision) { m_mapColInfo[ID] = bCollision; }

	void CheckGroup(OBJECTTAG _eLeft, OBJECTTAG _eRight);
	void Reset();

private:
	map<ULONGLONG, bool> m_mapColInfo;
	_uint	m_arrCheck[(_uint)OBJECTTAG::OBJECT_END];

	bool IsCollision(CCollider* _pLeft, CCollider* _pRight);
	void CheckCollisionByType(OBJECTTAG _eObjectLeft, OBJECTTAG _eObjectRight);

public:
	virtual void Free() override;
};

END