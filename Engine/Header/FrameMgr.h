#pragma once

#include "Base.h"
#include "Frame.h"

BEGIN(Engine)

class ENGINE_DLL CFrameMgr : public CBase
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	explicit CFrameMgr();
	virtual ~CFrameMgr();

public:
	_bool	Ispermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
	HRESULT Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);

private:
	map<const _tchar*, CFrame*>		m_mapFrame;

private:
	CFrame*		Find_Frame(const _tchar* pFrameTag);

public:
	virtual void	Free();
};

END
