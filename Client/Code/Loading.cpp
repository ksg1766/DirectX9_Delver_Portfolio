#include "stdafx.h"
#include "..\Header\Loading.h"
#include "Export_Function.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
}


CLoading::~CLoading()
{

}

unsigned int CLoading::Thread_Main(void * pArg)
{
	CLoading*		pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint	iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;
	}
	
	LeaveCriticalSection(pLoading->Get_Crt());

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	m_eID = eLoadingID;

	return S_OK;
}

_uint CLoading::Loading_ForStage()
{
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_PlayerBf", CPlayerBf::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_CubeBf", CCubeBf::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);

	m_bFinish = true;

	return 0;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading*		pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Loading Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}

