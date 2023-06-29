#pragma once

#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL CTimerMgr :	public CBase
{
	DECLARE_SINGLETON(CTimerMgr)

private:
	explicit CTimerMgr();
	virtual ~CTimerMgr();

private:
	CTimer*			Find_Timer(const _tchar* pTimerTag) const;

public:
	_float			Get_TimeDelta(const _tchar* pTimerTag);
	void			Set_TimeDelta(const _tchar* pTimerTag);

public:
	HRESULT			Ready_Timer(const _tchar* pTimerTag);

private:
	map<const _tchar*, CTimer*>		m_mapTimers;

public:
	virtual void	Free(void);
};

END