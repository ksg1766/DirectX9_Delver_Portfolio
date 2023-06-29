#pragma once

#include "Base.h"
#include "Engine_Define.h"

class CLoading : public CBase
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

public:
	CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	LOADINGID			Get_LoadingID() { return m_eID; }
	CRITICAL_SECTION*	Get_Crt()		{ return &m_Crt; }
	_bool				Get_Finish()	{ return m_bFinish; }

public:
	static unsigned	int		CALLBACK Thread_Main(void* pArg);

public:
	HRESULT			Ready_Loading(LOADINGID eLoadingID);
	_uint			Loading_ForStage();

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	LOADINGID			m_eID;

	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool				m_bFinish;

public:
	static		CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
	virtual void		Free();
};

