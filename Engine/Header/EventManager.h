#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CEventManager :
	public CBase
{
	DECLARE_SINGLETON(CEventManager)

private:
	explicit CEventManager();
	virtual ~CEventManager();

public:
	void Update();

	void AddEvent(const tagEvent& _eve) { m_vecEvent.push_back(_eve); }

	void CreateObject(CGameObject* _pObj, LAYERTAG _eLayer);
	void DeleteObject(CGameObject* _pObj);

private:
	void Execute(const tagEvent& _eve);

private:
	vector<tagEvent> m_vecEvent;
	vector<CGameObject*> m_vecCreate;
	vector<CGameObject*> m_vecDead;

public:
	virtual void Free() override;
};

END